#include "part.h"

#include "point_vector.h"
#include "point_vector_layer.h"

#include "base_math.h"

#include "ctrlSlideWidget.h"
#include "Joint.h"
#include "time_line_gl.h"
#include "piYingGL.h"
#include "PiYing.h"
#include "slide_applier.h"
#include "character_trace.h"
#include "global_objects.h"

#include <qopengltexture>
#include <qpointf>
#include <qmessagebox>
#include <qmenu>

Part::Part(
	QOpenGLTexture& texture,
	const QList<unsigned int>& indices,
	bool isTexture
) :
	_texture(texture)
{
	_slide_applier = new SlideApplier();
	_joint = std::make_unique<Joint>();

	_vert_texture = std::make_unique<PointVector>();
	_vert_texture_origin = std::make_unique<PointVector>();

	PointVectorLayer layer(*_vert_texture);

	std::vector<int> hashIndex(currentLayer->size(), -1);

	int eachIndex;
	int currentIndex = 0;

	const QPointF& firstPoint = (*currentLayer)[indices[0]];
	float top = firstPoint.y();
	float bottom = firstPoint.y();
	float left = firstPoint.x();
	float right = firstPoint.x();

	for (unsigned int i = 0; i < indices.size(); ++i) {
		eachIndex = hashIndex[indices[i]];
		if (eachIndex < 0) {
			hashIndex[indices[i]] = currentIndex;
			if (isTexture) {
				layer.push_back((*currentLayer)(indices[i]));

				top = cus::max(top, (*currentLayer)(indices[i]).y());
				bottom = cus::min(bottom, (*currentLayer)(indices[i]).y());
				left = cus::min(left, (*currentLayer)(indices[i]).x());
				right = cus::max(right, (*currentLayer)(indices[i]).x());
			}
			else {
				layer.push_back((*currentLayer)(indices[i]), (*currentLayer)[indices[i]]);

				top = cus::max(top, (*currentLayer)[indices[i]].y());
				bottom = cus::min(bottom, (*currentLayer)[indices[i]].y());
				left = cus::min(left, (*currentLayer)[indices[i]].x());
				right = cus::max(right, (*currentLayer)[indices[i]].x());
			}

			_indices.push_back(currentIndex++);
		}
		else {
			_indices.push_back(eachIndex);
		}
	}

	_vert_texture_origin->get_vector() = _vert_texture->get_vector();

	_x = (left + right) / 2.f;
	_y = (top + bottom) / 2.f;

	_height = top - bottom;
	_width = right - left;

	timelineGl->generate_vbo(*_vert_texture, _vbo);
	timelineGl->generate_ebo(_indices, _ebo);

	timelineGl->generate_vao(_vao_timeline, _vbo, _ebo);
	piYingGL->generate_vao(_vao_piying, _vbo, _ebo);
}

Part::Part(const Part& part1, const Part& part2) : _texture(part1._texture)
{
	_vert_texture_origin = std::make_unique<PointVector>(*part1._vert_texture_origin, *part2._vert_texture_origin);
	_vert_texture = std::make_unique<PointVector>(*part1._vert_texture, *part2._vert_texture);

	const unsigned int tempSize = (unsigned int)part1._vert_texture->half_point_size();
	_indices.reserve(part1._indices.size() + part2._indices.size());
	_indices.insert(_indices.end(), part1._indices.begin(), part1._indices.end());

	for (int i = 0; i < part2._indices.size(); ++i) {
		_indices.push_back(tempSize + part2._indices[i]);
	}

	_slide_applier = new SlideApplier(*part1._slide_applier, *part2._slide_applier, tempSize);

	timelineGl->generate_vbo(*_vert_texture, _vbo);
	timelineGl->generate_ebo(_indices, _ebo);

	timelineGl->generate_vao(_vao_timeline, _vbo, _ebo);
	piYingGL->generate_vao(_vao_piying, _vbo, _ebo);

	update_scale();
}

Part::Part(const Part& other) : 
	_x(other._x),
	_y(other._y),
	_height(other._height),
	_width(other._width),
	_texture(other._texture),
	_vert_texture_origin(std::make_unique<PointVector>(*other._vert_texture_origin)),
	_vert_texture(std::make_unique<PointVector>(*other._vert_texture)),
	_indices(other._indices),
	_slide_applier(new SlideApplier(*other._slide_applier)),
	_parent(other._parent),
	_joint(std::make_unique<Joint>()),
	localTransform(other.localTransform),
	worldTransform(other.worldTransform)
{
	static_assert(std::is_trivially_copyable_v<Joint>, "Must be trivially copyable");
	std::memcpy(_joint.get(), other._joint.get(), sizeof(Joint));

	timelineGl->generate_vbo(*_vert_texture, _vbo);
	timelineGl->generate_ebo(_indices, _ebo);

	timelineGl->generate_vao(_vao_timeline, _vbo, _ebo);
	piYingGL->generate_vao(_vao_piying, _vbo, _ebo); 

	for (size_t i = 0; i < other._children.size(); i++) {
		_children.push_back(new Part(*other._children[i]));
	}
}

Part::~Part()
{
	release_buffers();

	delete _slide_applier;
}

#pragma region [simple]

const float* Part::float_data() const
{
	return _vert_texture->data();
}

const unsigned int* Part::index_data() const
{
	return _indices.data();
}

size_t Part::float_size() const
{
	return _vert_texture->float_size();
}

size_t Part::index_size() const
{
	return _indices.size();
}

size_t Part::vertex_size() const
{
	return _vert_texture->point_size();
}

QPointF Part::get_vert(int index, bool isSkelen) const
{
	return (*_vert_texture)[index + index + (isSkelen ? 0 : 1)];
}

SlideApplier& Part::get_slide_applier()
{
	return *_slide_applier;
}

void Part::bind_texture()
{
	_texture.bind();
}

#pragma endregion

void Part::add_trace(int index, const QPolygonF& polygon)
{
	QMenu tempMenu;
	tempMenu.addAction(QString("新建控制器"));

	const unsigned int n_sliders = _slide_applier->n_sliders();
	for (unsigned int i = 0; i < n_sliders; ++i)
		tempMenu.addAction(QString("绑定到: %1").arg(_slide_applier->get_slider_name(i)));

	QAction* act = tempMenu.exec(QCursor::pos());
	if (!act) return;

	int actionIndex = tempMenu.actions().indexOf(act) - 1;

	if (actionIndex == -1) {
		_slide_applier->add_new_slider(index, polygon);
	}
	else {
		if (!_slide_applier->add_trace_on_exist_slider(actionIndex, index, polygon)) {
			QMessageBox::warning(nullptr, "警告", "该控制器已存在该点");
		}
	}
}

void Part::update_scale()
{
	PointVectorLayer layer(*_vert_texture);

	const QPointF& firstPoint = layer.get(0, true);
	float top = firstPoint.y();
	float bottom = firstPoint.y();
	float left = firstPoint.x();
	float right = firstPoint.x();

	QPointF eachPoint;
	for (unsigned int i = 1; i < layer.size(); ++i) {
		eachPoint = layer.get(i, true);
		top = cus::max(top, eachPoint.y());
		bottom = cus::min(bottom, eachPoint.y());
		left = cus::min(left, eachPoint.x());
		right = cus::max(right, eachPoint.x());
	}

	_x = (left + right) / 2.f;
	_y = (top + bottom) / 2.f;

	_height = top - bottom;
	_width = right - left;
}

bool Part::eat_another_part(Part& other)
{
	if (&_texture != &other._texture) {
		QMessageBox::warning(piYingGL, "警告", "不能合并不同纹理的部位");
		return false;
	}

	/// 拼接顶点下标vector
	const unsigned int tempSize = (unsigned int)_vert_texture_origin->half_point_size();
	_indices.reserve(_indices.size() + other._indices.size());
	for (int i = 0; i < other._indices.size(); ++i) {
		_indices.push_back(other._indices[i] + tempSize);
	}

	*_vert_texture_origin += *other._vert_texture_origin;	///< 拼接顶点坐标vector
	*_vert_texture += *other._vert_texture;					///< 拼接顶点坐标vector

	update_scale();

	_slide_applier->eat_other_sliders(other._slide_applier, tempSize);

	timelineGl->update_buffers(*_vert_texture, _indices, _vbo, _ebo);

	return true;
}

void Part::change_slider_value(int sliderIndex, int value)
{
	_slide_applier->change_current_value(sliderIndex, value);

	PointVectorLayer layer(*_vert_texture);
	PointVectorLayer layer_origin(*_vert_texture_origin);

	const std::map<unsigned int, QPolygonF>& tracesByPoint = _slide_applier->get_trace_map(sliderIndex);

	QPointF displacement;
	for (const auto& [key, val] : tracesByPoint) {
		displacement = QPointF();
		for (unsigned int i = 0; i < _slide_applier->n_sliders(); ++i) {
			const std::map<unsigned int, QPolygonF>& eachSlider = _slide_applier->get_trace_map(i);
			if (eachSlider.count(key)) {
				const QPolygonF& eachTrace = eachSlider.at(key);
				displacement += eachTrace[_slide_applier->get_slider_current_value(i) * (eachTrace.size() - 1) / 1000];
			}
		}
		layer.set_point(true, key, displacement + layer_origin.get(key, true)); 
	}

	timelineGl->update_vbo(*_vert_texture, _vbo);
	timelineGl->update();
	piYingGL->update();
}

void Part::remove_slider(int sliderIndex)
{
	_slide_applier->remove_slider(sliderIndex);
	piYing->update_part_slider();
}

void Part::release_buffers()
{
	timelineGl->release_buffers(_vao_timeline, _vbo, _ebo);
	piYingGL->release_buffers(_vao_piying);
}

void Part::add_child(Part* child)
{
	_children.push_back(child);
}

bool Part::same_texture_as(Part* other) const
{
	return &_texture == &other->_texture;
}

bool Part::is_root() const
{
	return !_parent;
}

Part* Part::get_child(size_t index) const
{
	return _children[index];
}

size_t Part::n_children() const
{
	return _children.size();
}

void Part::update_transform(const QMatrix4x4& parentWorld)
{
	localTransform = _joint->get_local_transform();
	worldTransform = parentWorld * localTransform;

	for (auto& child : _children) {
		if (child) child->update_transform(worldTransform);
	}
}

#pragma region [get value]

float Part::x() const
{
	return _x;
}

float Part::y() const
{
	return _y;
}

float Part::height() const
{
	return _height;
}

float Part::width() const
{
	return _width;
}

unsigned int Part::vao_timeline() const
{
	return _vao_timeline;
}

unsigned int Part::vao_piying() const
{
	return _vao_piying;
}

#pragma endregion