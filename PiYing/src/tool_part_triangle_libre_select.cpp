#include "tool_part_triangle_libre_select.h"

#include "tool_part_triangle_select.h"
#include "piYingGL.h"
#include "time_line_gl.h"
#include "selected_points.h"
#include "KeyboardStateWin.h"
#include "point_vector.h"

#include "enum_select_handle_mode.h"
#include "enum_edit_mode.h"

#include <qpolygonf>
#include <qpainter>
#include <array>

namespace {
	auto mapper = [](const QPointF& p) { return PiYingGL::getInstance().mapViewProjMatrix(p); };
}

piying::tool::part::TriangleLibreSelect::TriangleLibreSelect() :
	chTriangleSelect(std::make_unique<TriangleSelect>())
{
}

piying::tool::part::TriangleLibreSelect::~TriangleLibreSelect() {}

void piying::tool::part::TriangleLibreSelect::draw()
{
	chTriangleSelect->draw_handle_and_selected();

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

void piying::tool::part::TriangleLibreSelect::click(const QPointF& mouse)
{
	isPress = true;

	chTriangleSelect->mouse_press(mouse);

	polygon.clear();
	polygon << PiYingGL::getInstance().getViewProjMatrixInvert().map(PiYingGL::getInstance().mapToGL(mouse));
}

void piying::tool::part::TriangleLibreSelect::mouse_move(const QPointF& mouse)
{
	drawing = false;

	if (!isPress) return;

	if (chTriangleSelect->get_edit_mode() != HandleControlMode::None) {
		chTriangleSelect->moveHandle(mouse);
		return;
	}

	drawing = true;

	QPointF mapedMouse = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse);
	if (!polygon.isEmpty() && polygon.last() == mapedMouse) return;

	polygon << mapedMouse;
}

void piying::tool::part::TriangleLibreSelect::release(const QPointF& mouse)
{
	isPress = false;

	if (!drawing) {
		const QPointF glMouse = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse);

		chTriangleSelect->click_select(glMouse);
		return;
	}

	drawing = false;

	if (polygon.isEmpty()) return;

	polygon << polygon.first();

	if (!KeyboardStateWin::isCtrlHeld()) chTriangleSelect->selected_trangle->clear();

	const PointVectorLayer& pointVector = *TimelineGl::getInstance().currentLayerOrigin();
	const std::vector<unsigned int>& triangleIndices = *TimelineGl::getInstance().get_current_indices();

	QPointF eachTriangle[3];
	std::array<unsigned int, 3> triangle;
	for (unsigned int i = 0; i < triangleIndices.size(); i += 3) {
		for (int j = 0; j < 3; ++j) eachTriangle[j] = pointVector.get(triangleIndices[i + j], true);

		triangle = { triangleIndices[i] , triangleIndices[i + 1], triangleIndices[i + 2] };
		if (polygon.containsPoint(eachTriangle[0], Qt::OddEvenFill) &&
			polygon.containsPoint(eachTriangle[1], Qt::OddEvenFill) &&
			polygon.containsPoint(eachTriangle[2], Qt::OddEvenFill) &&
			!chTriangleSelect->selected_trangle->contains(triangle)
			) {
			chTriangleSelect->selected_trangle->append(triangle);
		}
	}

	chTriangleSelect->update_selected_to_draw();
}

void piying::tool::part::TriangleLibreSelect::delete_element()
{
	chTriangleSelect->deleteElement();
}

void piying::tool::part::TriangleLibreSelect::escape()
{
	chTriangleSelect->escape();
}