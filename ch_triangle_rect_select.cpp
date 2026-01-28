#include "ch_triangle_rect_select.h"

#include "ch_triangle_select.h"
#include "piYingGL.h"
#include "global_objects.h"
#include "selected_triangle.h"
#include "KeyboardStateWin.h"
#include "point_vector_layer.h"
#include "enum_select_handle_mode.h"
#include "enum_edit_mode.h"
#include "cus_func_zone.h"

#include <qpainter>
#include <qpointf>
#include <qmessagebox>

ChTriangleRectSelect::ChTriangleRectSelect() :
	edit_skelen(PiYingGL::getInstance().editMode == EditMode::characterSkeleton)
{
	chTriangleSelect = std::make_unique<ChTriangleSelect>();
}

void ChTriangleRectSelect::draw()
{
	chTriangleSelect->draw_handle_and_selected();

	QPainter painter(&PiYingGL::getInstance());
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QColor(225, 0, 0, 20));

	if (isDraw) {
		painter.setPen(QPen(Qt::yellow, 1));
		painter.drawRect(chTriangleSelect->lastPos.x(), chTriangleSelect->lastPos.y(), rect.x() - chTriangleSelect->lastPos.x(), rect.y() - chTriangleSelect->lastPos.y());
	}
}

void ChTriangleRectSelect::clickPos(const QPointF& mouseOri)
{
	chTriangleSelect->lastPos = mouseOri;

	chTriangleSelect->changeEditMode();

	if (chTriangleSelect->editMode != ChElementEditMode::None) {
		chTriangleSelect->affirmHandle();
	}
}

void ChTriangleRectSelect::movePos(const QPointF& mouse)
{
	if (chTriangleSelect->editMode != ChElementEditMode::None) {
		chTriangleSelect->moveHandle(mouse);
		isDraw = false;
		return;
	}

	rect = mouse;
	isDraw = true;
}

void ChTriangleRectSelect::releasePos(const QPointF& mouse)
{
	if (!isDraw) {
		const QPointF glMouse = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse);

		chTriangleSelect->click_select(glMouse);
		return;
	}
	isDraw = false;

	if (!KeyboardStateWin::isCtrlHeld()) chTriangleSelect->selected_trangle->clear();

	const PointVectorLayer& pointVector = *currentLayer;
	const std::vector<unsigned int>& triangleIndices = *currentIndex;

	QRectF rect(chTriangleSelect->lastPos, mouse);
	QPointF eachTriangle[3];
	const bool easySelect = mouse.x() > chTriangleSelect->lastPos.x();
	for (unsigned int i = 0; i < triangleIndices.size(); i += 3) {
		for (int j = 0; j < 3; ++j) eachTriangle[j] = PiYingGL::getInstance().mapViewProjMatrix(pointVector.get(triangleIndices[i + j], edit_skelen));

		if (rect.contains(eachTriangle[0]) &&
			rect.contains(eachTriangle[1]) &&
			rect.contains(eachTriangle[2]) &&
			!chTriangleSelect->selected_trangle->contains(&triangleIndices[i])
			) {
			chTriangleSelect->selected_trangle->append(&triangleIndices[i]);
		}
		else if (easySelect && isRectIntersectTriangle(rect, eachTriangle)) {
			if(!chTriangleSelect->selected_trangle->contains(&triangleIndices[i])) chTriangleSelect->selected_trangle->append(&triangleIndices[i]);
		}
	}

	chTriangleSelect->update_selected_to_draw();
}

void ChTriangleRectSelect::enter()
{
	chTriangleSelect->enter();
}

void RectSelectTriangleClick::click(const QPointF& point)
{
	rectSelect->clickPos(point);
}

void RectSelectTriangleMove::mouseMove(const QPointF& point)
{
	rectSelect->movePos(point);
}

void RectSelectTriangleRelease::release(const QPointF& point)
{
	rectSelect->releasePos(point);
}

void RectSelectTriangleDelete::deleteElement()
{
	rectSelect->chTriangleSelect->deleteElement();
}

void RectSelectTriangleEscape::escape()
{
	rectSelect->chTriangleSelect->escape();
}

void RectSelectTriangleDraw::draw()
{
	rectSelect->draw();
}

void RectSelectTriangleEnter::enter()
{
	rectSelect->enter();
}
