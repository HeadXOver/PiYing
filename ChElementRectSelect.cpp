#include "ChElementSelect.h"

QList<QPointF> ChElementrrRectSelect::getToDrawVert(int currentVector)
{
	QList<QPointF> vert;
	for (int i = 0; i < index.size(); i++) {
		vert.append(QPointF(glVert[currentVector][index[i] + index[i]], glVert[currentVector][index[i] + index[i] + 1]));
	}
	return vert;
}
