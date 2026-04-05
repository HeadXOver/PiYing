#include "tool_part_vert_libre_select.h"

#include "tool_part_vert_select.h"
#include "piYingGL.h"
#include "selected_points.h"
#include "KeyboardStateWin.h"
#include "point_vector.h"
#include "enum_select_handle_mode.h"
#include "enum_edit_mode.h"

#include <qpolygonf>
#include <qpainter>

namespace {
	auto mapper = [](const QPointF& p) { return PiYingGL::getInstance().mapViewProjMatrix(p); };
}

piying::tool::part::VertLibreSelect::VertLibreSelect() :
	edit_skelen(PiYingGL::getInstance().editMode == EditMode::characterSkeleton),
	_vert_select(std::make_unique<VertSelect>())
{
}

piying::tool::part::VertLibreSelect::~VertLibreSelect() {}

void piying::tool::part::VertLibreSelect::draw()
{
	_vert_select->draw_handle_and_selected();

	if (polygon.isEmpty() || !drawing) return;

	QPainter painter(&PiYingGL::getInstance());
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QColor(225, 0, 0, 20));
	painter.setPen(QPen(Qt::yellow, 1));

	QPolygonF screenPoly;
	screenPoly.reserve(polygon.size());
	std::transform(polygon.cbegin(), polygon.cend(),
		std::back_inserter(screenPoly),
		mapper
	);

	painter.drawPolyline(screenPoly);
	painter.drawLine(screenPoly.last(), screenPoly.first());
}

void piying::tool::part::VertLibreSelect::click(const QPointF& mouseOri)
{
	isPress = true;

	polygon.clear();

	_vert_select->change_edit_mode_by_setting_last_pos(mouseOri);

	if (_vert_select->edit_mode() != ToolHandleControlMode::None) {
		_vert_select->affirm_handle();
		return;
	}

	const QPointF mouse = PiYingGL::getInstance().getViewProjMatrixInvert().map(PiYingGL::getInstance().mapToGL(mouseOri));

	polygon << mouse;

	_vert_select->click_select(mouse);
}

void piying::tool::part::VertLibreSelect::escape()
{
	if (isPress) return;
	_vert_select->escape();
}

void piying::tool::part::VertLibreSelect::mouse_move(const QPointF& mouse)
{
	drawing = false;

	if (!isPress) return;

	if (_vert_select->edit_mode() != ToolHandleControlMode::None) {
		_vert_select->move_handle(mouse);
		return;
	}

	drawing = true;

	QPointF mapedMouse = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse);
	if (!polygon.isEmpty() && polygon.last() == mapedMouse) return;

	polygon << mapedMouse;
}

void piying::tool::part::VertLibreSelect::release(const QPointF& mouse)
{
	isPress = false;

	if (!drawing) return;

	drawing = false;

	if (polygon.isEmpty()) return;

	polygon << polygon.first();

	if (!KeyboardStateWin::isCtrlHeld()) _vert_select->clear();

	const PointVectorLayer& points = *PiYingGL::getInstance().currentLayer();
	QPointF existingPoint;
	for (unsigned int i = 0; i < points.element_size(); i++) {
		existingPoint = points.get(i, edit_skelen);
		if (polygon.containsPoint(existingPoint, Qt::OddEvenFill) && !_vert_select->contains(i)) {
			_vert_select->append(i);
		}
	}

	_vert_select->update_selected_to_draw();
}

void piying::tool::part::VertLibreSelect::delete_element()
{
	_vert_select->delete_element();
}