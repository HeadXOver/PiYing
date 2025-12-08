#include "ch_triangle_rect_select.h"

#include "ch_triangle_select.h"
#include "gl_vert_reference.h"
#include "piYingGL.h"
#include "global_objects.h"
#include "selected_triangle.h"
#include "KeyboardStateWin.h"
#include "point_vector_layer.h"
#include "enum_select_handle_mode.h"

#include <qpainter>
#include <qpointf>
#include <qmessagebox>

ChTriangleRectSelect::ChTriangleRectSelect(GlVertReference& glReference) :
	edit_skelen(piYingGL->editMode == EditMode::characterSkeleton)
{
	chTriangleSelect = std::make_unique<ChTriangleSelect>(glReference);
}

void ChTriangleRectSelect::draw()
{
	chTriangleSelect->draw_handle_and_selected();

	QPainter painter(piYingGL);
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
		return;
	}

	const QPointF mouse = piYingGL->GLViewProjMatrixInvert(mouseOri);

	chTriangleSelect->click_select(mouse);
}

void ChTriangleRectSelect::movePos(const QPointF& mouse)
{
	isDraw = false;

	if (chTriangleSelect->editMode != ChElementEditMode::None) {
		chTriangleSelect->moveHandle(mouse);
		return;
	}

	rect = mouse;
	isDraw = true;
}

void ChTriangleRectSelect::releasePos(const QPointF& mouse)
{
	if (!isDraw) return;
	isDraw = false;

	chTriangleSelect->selected_trangle->clear();

	PointVectorLayer& pointVector = *(chTriangleSelect->glVertReference.pointLayer);
	for (unsigned int i = 0; i < pointVector.size(); i++) {
		if (QRectF(chTriangleSelect->lastPos, mouse).contains(piYingGL->mapViewProjMatrix(
			edit_skelen ?
			pointVector[i] :
			pointVector(i))
		)) chTriangleSelect->selected_trangle->append(&i);
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
