#include "ChElementSelect.h"
#include "piYingGL.h"

void ChElementRectSelect::draw(QPainter& painter)
{
	for (int i = 0; i < index.size(); i++) {
		QPointF selectPoint = gl->mapViewProjMatrix(sVert[index[i]]);
		painter.setPen(QPen(Qt::black, 8));
		painter.drawPoint(selectPoint);
		painter.setPen(QPen(Qt::red, 6));
		painter.drawPoint(selectPoint);
	}

	if (isDraw) {
		QPointF selectPoint = gl->mapViewProjMatrix(lastPos);
		painter.setPen(QPen(Qt::yellow, 1));
		painter.drawRect(selectPoint.x(), selectPoint.y(), rect.x() - selectPoint.x(), rect.y() - selectPoint.y());
	}
}

void ChElementRectSelect::clickPos(const QPointF& mouse)
{
	lastPos = mouse;
	isPress = true;
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

	rect = gl->mapViewProjMatrix(mouse);
	isDraw = true;
}

void ChElementRectSelect::releasePos(const QPointF& mouse)
{
	isPress = false;
	if (!isDraw) return;
	isDraw = false;

	const std::vector<float>& glV = glVert;
	index.clear();

	for (unsigned int i = 0; i < glV.size(); i += 2) {
		float front = glV[i] - lastPos.x();
		float back = glV[i] - mouse.x();

		if ((front > 0 && back > 0) || (front < 0 && back < 0)) {
			continue;
		}

		front = glV[i + 1] - lastPos.y();
		back = glV[i + 1] - mouse.y();

		if ((front > 0 && back > 0) || (front < 0 && back < 0)) {
			continue;
		}

		index.append(i / 2);
	}
}
