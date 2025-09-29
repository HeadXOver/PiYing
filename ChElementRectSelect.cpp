#include "ChElementSelect.h"
#include "piYingGL.h"

void ChElementrrRectSelect::draw(QPainter& painter)
{
	for (int i = 0; i < index.size(); i++) {
		int ind = index[i];
		ind += ind;
		QPointF selectPoint = gl->mapViewProjMatrix(QPointF(glVert[currentVector][ind], glVert[currentVector][ind + 1]));
		painter.setPen(QPen(Qt::black, 8));
		painter.drawPoint(selectPoint);
		painter.setPen(QPen(Qt::red, 6));
		painter.drawPoint(selectPoint);
	}
}

void ChElementrrRectSelect::movePos(const QPointF& mouse)
{
	
}
