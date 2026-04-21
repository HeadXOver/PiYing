#include "tool_add_line_trace.h"

#include "piYingGL.h"
#include "time_line_gl.h"
#include "part.h"

#include <qpainter>

void piying::tool::part::AddLineTrace::click(const QPointF& mouseOri)
{
	const QPointF mouse = PiYingGL::getInstance().GLViewProjMatrixInvert(mouseOri);

	Part* part = TimelineGl::getInstance().get_current_part();

	assert(part != nullptr);

	for (unsigned int i = 0; i < part->vertex_size(); i++) {
		_origin_vert_pos = part->get_vert(i, true);
		if (QLineF(_origin_vert_pos, mouse).length() < 0.02f / PiYingGL::getInstance().viewScale.value()) {
			current_index = i;
			_current_mouse_pos = mouseOri;

			return;
		}
	}

	current_index = -1;
}

void piying::tool::part::AddLineTrace::mouse_move(const QPointF& mouse)
{
	if (current_index < 0) return;

	_current_mouse_pos = mouse;
}

void piying::tool::part::AddLineTrace::release(const QPointF& mouse)
{
	if (current_index < 0) return;

	QPointF glMouse = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse);

	if (QLineF(_origin_vert_pos, glMouse).length() < 0.06f / PiYingGL::getInstance().viewScale.value()) {
		current_index = -1; 
		return;
	}

	Part* part = TimelineGl::getInstance().get_current_part();
	if (part) part->add_trace(current_index, PiYingGL::getInstance().GLViewProjMatrixInvert(_current_mouse_pos) - _origin_vert_pos);

	current_index = -1;
}

void piying::tool::part::AddLineTrace::draw()
{
	if (current_index < 0) return;

	QPointF selectPoint = PiYingGL::getInstance().mapViewProjMatrix(TimelineGl::getInstance().get_current_part()->get_vert(current_index, true));

	QPainter painter(&PiYingGL::getInstance());
	painter.setPen(QPen(Qt::black, 8));
	painter.drawPoint(selectPoint);
	painter.setPen(QPen(Qt::red, 6));
	painter.drawPoint(selectPoint);

	painter.setPen(QPen(Qt::yellow, 1));
	painter.drawLine(selectPoint, _current_mouse_pos);
}
