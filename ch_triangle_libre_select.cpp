#include "ch_triangle_libre_select.h"

#include "ch_triangle_select.h"
#include "piYingGL.h"
#include "selected_triangle.h"
#include "KeyboardStateWin.h"
#include "point_vector_layer.h"

#include "enum_select_handle_mode.h"
#include "enum_edit_mode.h"

#include <qpolygonf>
#include <qpainter>

namespace {
	auto mapper = [](const QPointF& p) { return PiYingGL::getInstance().mapViewProjMatrix(p); };
}

ChTriangleLibreSelect::ChTriangleLibreSelect() :
	edit_skelen(PiYingGL::getInstance().editMode == EditMode::characterSkeleton)
{
	chTriangleSelect = std::make_unique<ChTriangleSelect>();
}

void ChTriangleLibreSelect::enter()
{
	chTriangleSelect->enter();
}

void ChTriangleLibreSelect::draw()
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

void ChTriangleLibreSelect::click(const QPointF& mouseOri)
{
	isPress = true;
	chTriangleSelect->lastPos = mouseOri;

	polygon.clear();

	chTriangleSelect->changeEditMode();

	if (chTriangleSelect->get_edit_mode() != ChElementEditMode::None) {
		chTriangleSelect->affirmHandle();
		return;
	}
	const QPointF mouse = PiYingGL::getInstance().getViewProjMatrixInvert().map(PiYingGL::getInstance().mapToGL(mouseOri));

	polygon << mouse;
}

void ChTriangleLibreSelect::mouse_move(const QPointF& mouse)
{
	drawing = false;

	if (chTriangleSelect->get_edit_mode() != ChElementEditMode::None) {
		chTriangleSelect->moveHandle(mouse);
		return;
	}

	if (!isPress) return;

	drawing = true;

	QPointF mapedMouse = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse);
	if (!polygon.isEmpty() && polygon.last() == mapedMouse) return;

	polygon << mapedMouse;
}

void ChTriangleLibreSelect::release(const QPointF& mouse)
{
	isPress = false;

	if (!drawing) {
		const QPointF glMouse = PiYingGL::getInstance().getViewProjMatrixInvert().map(PiYingGL::getInstance().mapToGL(mouse));

		chTriangleSelect->click_select(glMouse);
		return;
	}

	drawing = false;

	if (polygon.isEmpty()) return;

	polygon << polygon.first();

	if (!KeyboardStateWin::isCtrlHeld()) chTriangleSelect->selected_trangle->clear();

	const PointVectorLayer& pointVector = *PiYingGL::getInstance().currentLayer();
	const std::vector<unsigned int>& triangleIndices = *PiYingGL::getInstance().currentIndex();

	QPointF eachTriangle[3];
	for (unsigned int i = 0; i < triangleIndices.size(); i += 3) {
		for (int j = 0; j < 3; ++j) eachTriangle[j] = pointVector.get(triangleIndices[i + j], edit_skelen);

		if (polygon.containsPoint(eachTriangle[0], Qt::OddEvenFill) &&
			polygon.containsPoint(eachTriangle[1], Qt::OddEvenFill) &&
			polygon.containsPoint(eachTriangle[2], Qt::OddEvenFill) &&
			!chTriangleSelect->selected_trangle->contains(&triangleIndices[i])
			) {
			chTriangleSelect->selected_trangle->append(&triangleIndices[i]);
		}
	}

	chTriangleSelect->update_selected_to_draw();
}

void ChTriangleLibreSelect::delete_element()
{
	chTriangleSelect->deleteElement();
}

void ChTriangleLibreSelect::escape()
{
	chTriangleSelect->escape();
}