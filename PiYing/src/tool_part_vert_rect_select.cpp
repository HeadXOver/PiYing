#include "tool_part_vert_rect_select.h"

#include "tool_part_vert_select.h"
#include "piYingGL.h"
#include "part.h"
#include "time_line_gl.h"
#include "KeyboardStateWin.h"
#include "point_vector.h"

#include "enum_select_handle_mode.h"

#include <qpainter>

piying::tool::part::VertRectSelect::VertRectSelect() :
	_vert_select(std::make_unique<VertSelect>()) 
{
	
}

piying::tool::part::VertRectSelect::~VertRectSelect() {}

void piying::tool::part::VertRectSelect::draw()
{
	_vert_select->draw_handle_and_selected();

	QPainter painter(&PiYingGL::getInstance());
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QColor(225, 0, 0, 20));
	
	if (isDraw) {
		painter.setPen(QPen(Qt::yellow, 1));
		const QPointF& lastPos = _vert_select->get_last_pos();
		painter.drawRect(lastPos.x(), lastPos.y(), rect.x() - lastPos.x(), rect.y() - lastPos.y());
	}
}

void piying::tool::part::VertRectSelect::delete_element()
{
	_vert_select->delete_element();
}

void piying::tool::part::VertRectSelect::click(const QPointF& mouse)
{
	_is_press = true;
	_vert_select->change_edit_mode_by_setting_last_pos(mouse);

	if (_vert_select->edit_mode() != ToolHandleControlMode::None) {
		_vert_select->affirm_handle();
		return;
	}

	_vert_select->click_select(
		PiYingGL::getInstance().GLViewProjMatrixInvert(mouse)
	);
}

void piying::tool::part::VertRectSelect::escape()
{
	_vert_select->escape();
}

void piying::tool::part::VertRectSelect::mouse_move(const QPointF& mouse)
{
	if (!_is_press) return;

	if (_vert_select->edit_mode() != ToolHandleControlMode::None) {
		_vert_select->move_handle(mouse);
		isDraw = false;
		return;
	}

	rect = mouse;
	isDraw = true;
}

void piying::tool::part::VertRectSelect::release(const QPointF& mouse)
{
	_is_press = false;
	TimelineGl::getInstance().update_current_part_scale();
	if (!isDraw) return;
	isDraw = false;

	if (!KeyboardStateWin::isCtrlHeld()) _vert_select->clear();

	const QRectF rect(_vert_select->get_last_pos(), mouse);
	const PointVectorLayer& pointVector = *TimelineGl::getInstance().currentLayerOrigin();
	QPointF pos;
	for (unsigned int i = 0; i < pointVector.element_size(); i++) {
		if (TimelineGl::getInstance().get_current_part()->is_point_traced(i)) continue;
		pos = PiYingGL::getInstance().mapViewProjMatrix(pointVector.get(i, true));
		if (rect.contains(pos) && !_vert_select->contains(i)) {
			_vert_select->append(i);
		}
	}

	_vert_select->update_selected_to_draw();
}
