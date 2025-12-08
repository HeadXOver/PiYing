#include "ch_element_rect_select.h"

#include "ch_element_select.h"
#include "gl_vert_reference.h"
#include "piYingGL.h"
#include "global_objects.h"
#include "SelectedPoints.h"
#include "KeyboardStateWin.h"
#include "point_vector_layer.h"
#include "enum_select_handle_mode.h"

#include <qpainter>
#include <qpointf>
#include <qmessagebox>

ChElementRectSelect::ChElementRectSelect(GlVertReference& glReference) :
	edit_skelen(piYingGL->editMode == EditMode::characterSkeleton)
{
	chElementSelect = std::make_unique<ChElementSelect>(glReference);
}

void ChElementRectSelect::draw()
{
	chElementSelect->draw_handle_and_selected();

	QPainter painter(piYingGL);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QColor(225, 0, 0, 20));

	if (isDraw) {
		painter.setPen(QPen(Qt::yellow, 1));
		painter.drawRect(chElementSelect->lastPos.x(), chElementSelect->lastPos.y(), rect.x() - chElementSelect->lastPos.x(), rect.y() - chElementSelect->lastPos.y());
	}
}

void ChElementRectSelect::clickPos(const QPointF& mouseOri)
{
	chElementSelect->lastPos = mouseOri;

	chElementSelect->changeEditMode();

	if (chElementSelect->editMode != ChElementEditMode::None) {
		chElementSelect->affirmHandle();
		return;
	}

	const QPointF mouse = piYingGL->GLViewProjMatrixInvert(mouseOri);

	chElementSelect->click_select(mouse);
}

void ChElementRectSelect::movePos(const QPointF& mouse)
{
	if (chElementSelect->editMode != ChElementEditMode::None) {
		chElementSelect->moveHandle(mouse);
		isDraw = false;
		return;
	}

	rect = mouse;
	isDraw = true;
}

void ChElementRectSelect::releasePos(const QPointF& mouse)
{
	if (!isDraw) return;
	isDraw = false;

	chElementSelect->selected_points->clear();

	const QRectF rect(chElementSelect->lastPos, mouse);
	const PointVectorLayer& pointVector = *(chElementSelect->glVertReference.pointLayer);
	QPointF pos;
	for (unsigned int i = 0; i < pointVector.size(); i++) {
		pos = piYingGL->mapViewProjMatrix(pointVector.get(i, edit_skelen));
		if (rect.contains(pos)) {
			chElementSelect->selected_points->append(i);
		}
	}

	chElementSelect->update_selected_to_draw();
}

void ChElementRectSelect::enter()
{
	chElementSelect->enter();
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

void RectSelectDraw::draw()
{
	rectSelect->draw();
}

void RectSelectEnter::enter()
{
	rectSelect->enter();
}
