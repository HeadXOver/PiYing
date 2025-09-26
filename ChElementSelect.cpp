#include "ChElementSelect.h"

void ChElementSelect::deleteElement(int currentVector)
{
	for (int i = 0; i < index.size(); i++) {
		for (int j = (int)glIndex[currentVector].size() - 3; j >= 0 ; j -= 3) {
			if (glIndex[currentVector][j] == index[i] || glIndex[currentVector][j + 1] == index[i] || glIndex[currentVector][j + 2] == index[i]) {
				glIndex[currentVector].erase(glIndex[currentVector].begin() + j, glIndex[currentVector].begin() + j + 3);
			}
		}
	}
	index.clear();
}

void ChElementSelect::clickPos(const QPointF& mouse, float viewScale, int currentVector)
{
	for (unsigned int i = 0; i < glVert[currentVector].size() / 2; i++) {
		QPointF readyPoint(glVert[currentVector][i + i], glVert[currentVector][i + i + 1]);
		if (QLineF(readyPoint, mouse).length() < 0.02f / viewScale) {
			index.append(i);
			return;
		}
	}

	index.clear();
}

QList<QPointF> ChElementSelect::getToDrawVert(int currentVector)
{
	QList<QPointF> vert;
	for (int i = 0; i < index.size(); i++) {
		vert.append(QPointF(glVert[currentVector][index[i] + index[i]], glVert[currentVector][index[i] + index[i] + 1]));
	}
	return vert;
}
