#include "ChElementSelect.h"

#include "chElementRectSelect.h"
#include "glVertReference.h"
#include "piYingGL.h"
#include "SelectedPoints.h"
#include "KeyboardStateWin.h"

#include <qpainter>

ChElementRectSelect::ChElementRectSelect(GlVertReference* glReference) :chElementSelect(new ChElementSelect(glReference))
{
}

void ChElementRectSelect::draw(QPainter* painter)
{
	chElementSelect->drawHandle(painter);

	for (int i = 0; i < chElementSelect->selectedPoints->size(); i++) {
		QPointF selectPoint = chElementSelect->glVertReference->gl->mapViewProjMatrix(chElementSelect->glVertReference->sVert[(*chElementSelect->selectedPoints)[i]]);
		painter->setPen(QPen(Qt::black, 8));
		painter->drawPoint(selectPoint);
		painter->setPen(QPen(Qt::red, 6));
		painter->drawPoint(selectPoint);
	}

	if (isDraw) {
		painter->setPen(QPen(Qt::yellow, 1));
		painter->drawRect(chElementSelect->lastPos.x(), chElementSelect->lastPos.y(), rect.x() - chElementSelect->lastPos.x(), rect.y() - chElementSelect->lastPos.y());
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

	QPointF mouse = chElementSelect->glVertReference->gl->getViewProjMatrixInvert().map(chElementSelect->glVertReference->gl->mapToGL(mouseOri));

	for (unsigned int i = 0; i < chElementSelect->glVertReference->sVert.size(); i++) {
		if (QLineF(chElementSelect->glVertReference->sVert[i], mouse).length() < 0.02f / chElementSelect->glVertReference->gl->viewScale.value()) {
			if (!chElementSelect->selectedPoints->contains(i)) {
				if (!KeyboardStateWin::isCtrlHeld()) {
					chElementSelect->selectedPoints->clear();
				}
				chElementSelect->selectedPoints->append(i);
			}
			return;
		}
	}

	if (!KeyboardStateWin::isCtrlHeld()) {
		chElementSelect->selectedPoints->clear();
	}
}

void ChElementRectSelect::movePos(const QPointF& mouse)
{
	isDraw = false;
	
	if (chElementSelect->editMode == ChElementEditMode::None) {
		rect = mouse;
		isDraw = true;
	}
	else chElementSelect->moveHandle(mouse);
}

void ChElementRectSelect::releasePos(const QPointF& mouse)
{
	chElementSelect->isPress = false;

	if (!isDraw) return;
	isDraw = false;

	chElementSelect->selectedPoints->clear();

	for (unsigned int i = 0; i < chElementSelect->glVertReference->sVert.size(); i++)
		if (QRectF(chElementSelect->lastPos, mouse).contains(chElementSelect->glVertReference->gl->mapViewProjMatrix(chElementSelect->glVertReference->sVert[i])))
			chElementSelect->selectedPoints->append(i);
		
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

void RectSelectDraw::draw(QPainter* painter)
{
	rectSelect->draw(painter);
}
