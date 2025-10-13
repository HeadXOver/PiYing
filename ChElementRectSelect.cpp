#include "ChElementSelect.h"
#include "piYingGL.h"

void ChElementRectSelect::draw(QPainter& painter)
{
	drawHandle(painter);

	for (int i = 0; i < index.size(); i++) {
		QPointF selectPoint = gl->mapViewProjMatrix(sVert[index[i]]);
		painter.setPen(QPen(Qt::black, 8));
		painter.drawPoint(selectPoint);
		painter.setPen(QPen(Qt::red, 6));
		painter.drawPoint(selectPoint);
	}

	if (isDraw) {
		painter.setPen(QPen(Qt::yellow, 1));
		painter.drawRect(lastPos.x(), lastPos.y(), rect.x() - lastPos.x(), rect.y() - lastPos.y());
	}
}

void ChElementRectSelect::clickPos(const QPointF& mouseOri)
{
	lastPos = mouseOri;
	isPress = true;

	changeEditMode();

	if(editMode != ChElementEditMode::None) return;

	QPointF mouse = gl->getViewProjMatrixInvert().map(gl->mapToGL(mouseOri));

	for (unsigned int i = 0; i < sVert.size(); i++) {
		if (QLineF(sVert[i], mouse).length() < 0.02f / gl->viewScale.value()) {
			if (!index.contains(i)) {
				if (!KeyboardStateWin::isCtrlHeld()) {
					index.clear();
				}
				index.append(i);
			}
			return;
		}
	}

	if (!KeyboardStateWin::isCtrlHeld()) {
		index.clear();
	}
}

void ChElementRectSelect::movePos(const QPointF& mouse)
{
	isDraw = false;
	
	if (editMode == ChElementEditMode::None) {
		rect = mouse;
		isDraw = true;
	}
	else moveHandle(mouse);
}

void ChElementRectSelect::releasePos(const QPointF& mouse)
{
	isPress = false;
	if (!isDraw) return;
	isDraw = false;

	index.clear();

	for (unsigned int i = 0; i < sVert.size(); i++) 
		if(QRectF(lastPos, mouse).contains(gl->mapViewProjMatrix(sVert[i])))
			index.append(i);
}
