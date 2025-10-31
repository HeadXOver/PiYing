#include "ch_element_select.h"

#include "ch_element_rect_select.h"
#include "gl_vert_reference.h"
#include "piYingGL.h"
#include "SelectedPoints.h"
#include "KeyboardStateWin.h"
#include "point_vector_layer.h"

#include <qpainter>
#include <qpointf>

ChElementRectSelect::ChElementRectSelect(GlVertReference& glReference) :edit_skelen(glReference.gl.editMode == EditMode::characterSkeleton)
{
	chElementSelect = std::make_unique<ChElementSelect>(glReference);
}

void ChElementRectSelect::draw(QPainter& painter)
{
	chElementSelect->draw_handle_and_selected(painter);

	if (isDraw) {
		painter.setPen(QPen(Qt::yellow, 1));
		painter.drawRect(chElementSelect->lastPos.x(), chElementSelect->lastPos.y(), rect.x() - chElementSelect->lastPos.x(), rect.y() - chElementSelect->lastPos.y());
	}
}

void ChElementRectSelect::clickPos(const QPointF& mouseOri)
{
	chElementSelect->lastPos = mouseOri;
	chElementSelect->isPress = true;

	chElementSelect->changeEditMode();

	if (chElementSelect->editMode != ChElementEditMode::None) {
		chElementSelect->affirmHandle();
		return;
	}

	const QPointF mouse = chElementSelect->glVertReference.gl.GLViewProjMatrixInvert(mouseOri);

	chElementSelect->click_select(mouse);
}

void ChElementRectSelect::movePos(const QPointF& mouse)
{
	isDraw = false;

	if (chElementSelect->editMode != ChElementEditMode::None) {
		chElementSelect->moveHandle(mouse);
		return;
	}

	rect = mouse;
	isDraw = true;
}

void ChElementRectSelect::releasePos(const QPointF& mouse)
{
	chElementSelect->isPress = false;

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
}

void RectSelectClick::click(const QPointF& point)
{
	rectSelect->clickPos(point);
}

void RectSelectMove::mouseMove(const QPointF& point)
{
	rectSelect->movePos(point);
}

void RectSelectRelease::release(const QPointF& point)
{
	rectSelect->releasePos(point);
}

void RectSelectDelete::deleteElement()
{
	rectSelect->chElementSelect->deleteElement();
}

void RectSelectEscape::escape()
{
	rectSelect->chElementSelect->escape();
}

void RectSelectDraw::draw(QPainter& painter)
{
	rectSelect->draw(painter);
}
