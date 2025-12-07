#include "ch_triangle_rect_select.h"

#include "ch_element_select.h"
#include "gl_vert_reference.h"
#include "piYingGL.h"
#include "SelectedPoints.h"
#include "KeyboardStateWin.h"
#include "point_vector_layer.h"
#include "enum_select_handle_mode.h"

#include <qpainter>
#include <qpointf>
#include <qmessagebox>

ChTriangleRectSelect::ChTriangleRectSelect(GlVertReference& glReference) :
	edit_skelen(glReference.gl.editMode == EditMode::characterSkeleton)
{
	chElementSelect = std::make_unique<ChElementSelect>(glReference);
}

void ChTriangleRectSelect::draw(QPainter& painter)
{
	chElementSelect->draw_handle_and_selected(painter);

	if (isDraw) {
		painter.setPen(QPen(Qt::yellow, 1));
		painter.drawRect(chElementSelect->lastPos.x(), chElementSelect->lastPos.y(), rect.x() - chElementSelect->lastPos.x(), rect.y() - chElementSelect->lastPos.y());
	}
}

void ChTriangleRectSelect::clickPos(const QPointF& mouseOri)
{
	chElementSelect->lastPos = mouseOri;

	chElementSelect->changeEditMode();

	if (chElementSelect->editMode != ChElementEditMode::None) {
		chElementSelect->affirmHandle();
		return;
	}

	const QPointF mouse = chElementSelect->glVertReference.gl.GLViewProjMatrixInvert(mouseOri);

	chElementSelect->click_select(mouse);
}

void ChTriangleRectSelect::movePos(const QPointF& mouse)
{
	isDraw = false;

	if (chElementSelect->editMode != ChElementEditMode::None) {
		chElementSelect->moveHandle(mouse);
		return;
	}

	rect = mouse;
	isDraw = true;
}

void ChTriangleRectSelect::releasePos(const QPointF& mouse)
{
	if (!isDraw) return;
	isDraw = false;

	chElementSelect->selected_points->clear();

	PointVectorLayer& pointVector = *(chElementSelect->glVertReference.pointLayer);
	for (unsigned int i = 0; i < pointVector.size(); i++) {
		if (QRectF(chElementSelect->lastPos, mouse).contains(chElementSelect->glVertReference.gl.mapViewProjMatrix(
			edit_skelen ?
			pointVector[i] :
			pointVector(i))
		)) chElementSelect->selected_points->append(i);
	}

	chElementSelect->uodate_selected_to_draw();
}

void ChTriangleRectSelect::enter()
{
	chElementSelect->enter();
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
	rectSelect->chElementSelect->deleteElement();
}

void RectSelectTriangleEscape::escape()
{
	rectSelect->chElementSelect->escape();
}

void RectSelectTriangleDraw::draw(QPainter& painter)
{
	rectSelect->draw(painter);
}

void RectSelectTriangleEnter::enter()
{
	rectSelect->enter();
}
