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
	slide_applier = new SlideApplier();
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

	slide_applier = new SlideApplier(*part1.slide_applier, *part2.slide_applier, tempSize);

	timelineGl->generate_vbo(*_vert_texture, _vbo);
	timelineGl->generate_ebo(_indices, _ebo);

	timelineGl->generate_vao(_vao_timeline, _vbo, _ebo);
	piYingGL->generate_vao(_vao_piying, _vbo, _ebo);

	update_scale();
}

Part::Part(const Part& other) : 
	_texture(other._texture),
	_x(other._x),
	_y(other._y),
	_height(other._height),
	_width(other._width),
	_vert_texture_origin(std::make_unique<PointVector>(*other._vert_texture_origin)),
	_vert_texture(std::make_unique<PointVector>(*other._vert_texture)),
	_indices(other._indices)
{
	slide_applier = new SlideApplier(*other.slide_applier);

	timelineGl->generate_vbo(*_vert_texture, _vbo);
	timelineGl->generate_ebo(_indices, _ebo);

	timelineGl->generate_vao(_vao_timeline, _vbo, _ebo);
	piYingGL->generate_vao(_vao_piying, _vbo, _ebo);
}

Part::~Part()
{
	delete slide_applier;
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
	return *slide_applier;
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

	const unsigned int n_sliders = slide_applier->n_sliders();
	for (unsigned int i = 0; i < n_sliders; ++i)
		tempMenu.addAction(QString("绑定到: %1").arg(slide_applier->get_slider_name(i)));

	QAction* act = tempMenu.exec(QCursor::pos());
	if (!act) return;

	int actionIndex = tempMenu.actions().indexOf(act) - 1;

	if (actionIndex == -1) {
		slide_applier->add_new_slider(index, polygon);
	}
	else {
		if (!slide_applier->add_trace_on_exist_slider(actionIndex, index, polygon)) {
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

	slide_applier->eat_other_sliders(other.slide_applier, tempSize);

	timelineGl->update_buffers(*_vert_texture, _indices, _vbo, _ebo);

	return true;
}

void Part::change_slider_value(int sliderIndex, int value)
{
	slide_applier->change_current_value(sliderIndex, value);

	PointVectorLayer layer(*_vert_texture);
	PointVectorLayer layer_origin(*_vert_texture_origin);

	const std::map<unsigned int, QPolygonF>& tracesByPoint = slide_applier->get_trace_map(sliderIndex);

	QPointF displacement;
	for (const auto& [key, val] : tracesByPoint) {
		displacement = QPointF();
		for (unsigned int i = 0; i < slide_applier->n_sliders(); ++i) {
			const std::map<unsigned int, QPolygonF>& eachSlider = slide_applier->get_trace_map(i);
			if (eachSlider.count(key)) {
				const QPolygonF& eachTrace = eachSlider.at(key);
				displacement += eachTrace[slide_applier->get_slider_current_value(i) * (eachTrace.size() - 1) / 1000];
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
	slide_applier->remove_slider(sliderIndex);
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

void Part::add_to_draw()
{
	partIsDraw[index] = true;

	for (int i = 0; i < _children.size(); i++) {
		if (_children[i]) _children[i]->add_to_draw();
	}
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