#include "part.h"

#include "point_vector.h"
#include "point_vector_layer.h"

#include "base_math.h"

#include "ctrlSlideWidget.h"
#include "time_line_gl.h"
#include "piYingGL.h"
#include "PiYing.h"
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
	_sliderWidget = new CtrlSlideWidget(*piYingGL, "part");
	_sliderWidget->setStyleSheet(PiYing::SLIDER_WIDGET_STYLE_SHEET);
	_sliderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	_vert_texture = std::make_unique<PointVector>();
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

	_x = (left + right) / 2.f;
	_y = (top + bottom) / 2.f;

	_height = top - bottom;
	_width = right - left;

	timelineGl->generate_vbo(*_vert_texture, _vbo);
	timelineGl->generate_ebo(_indices, _ebo);
	timelineGl->generate_vao(_vao_timeline, _vbo, _ebo);

	piYingGL->generate_ebo(_indices, _ebo);
	piYingGL->generate_vao(_vao_piying, _vbo, _ebo);
}

float* Part::float_data() const
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
	return _vert_texture->size();
}

QPointF Part::get_vert(int index, bool isSkelen) const
{
	return (*_vert_texture)[index + index + (isSkelen ? 0 : 1)];
}

void Part::bind_texture()
{
	_texture.bind();
}

void Part::add_trace(int index, const QPolygonF& polygon)
{
	QList<QString> items = _sliderWidget->get_slider_names();

	QMenu tempMenu(piYingGL);
	tempMenu.addAction(QString("新建控制器"));

	for (int i = 0; i < items.size(); ++i)
		tempMenu.addAction(QString("绑定到: %1").arg(items[i]));

	QAction* act = tempMenu.exec(QCursor::pos());
	if (!act) return;

	int id = tempMenu.actions().indexOf(act) - 1;

	if (id == -1) {
		_sliderWidget->addSlider();
		id = items.size();
	}

	if (!_sliderWidget->add_trace(_sliderWidget->get_id(id), index, polygon)) QMessageBox::warning(piYingGL, "警告", "轨迹重复");
}

void Part::apply_slide(const std::map<int, std::unique_ptr<CharacterTrace>>& traces, int value)
{
	PointVectorLayer layer(*_vert_texture);

	for (const auto& [key, val] : traces) {
		layer.set_point(true, key, val->get_point(value));
	}

	timelineGl->update_vbo(*_vert_texture, _vbo);
	timelineGl->update();
	piYingGL->update();
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

CtrlSlideWidget* Part::slider_widget() const
{
	return _sliderWidget;
}

#pragma endregion