#include "AddChTexPoly.h"
#include "piYingGL.h"

void AddChTexPoly::escape()
{
	if (index.size() == 0) return;

	index.removeLast();
	points.removeLast();
}

void AddChTexPoly::enter()
{
	if (index.size() < 3) return;

	for (int i = 0; i < index.size(); i++) {
		if (index[i] < 0) {
			index[i] = (int)sVert.size();
			addPointToVert(points[i]);
		}
	}

	int last2point[2] = { 0,1 };
	int front = 2;
	int back = index.size() - 1;
	bool isBack = true;
	for (int i = 2; i < index.size(); i++) {
		int v[3] = { last2point[0], last2point[1], isBack ? back : front };
		for (int j = 0; j < 3; j++) {
			glIndex.push_back(index[v[j]]);
		}
		last2point[0] = v[1];
		last2point[1] = v[2];
		if(isBack) back--;
		else front++;
		isBack = !isBack;
	}

	index.clear();
	points.clear();
}

void AddChTexPoly::deleteElement()
{
	index.clear();
	points.clear();
}

void AddChTexPoly::clickPos(const QPointF& mouseOri)
{
	QPointF mouse = gl->getViewProjMatrixInvert().map(gl->mapToGL(mouseOri));

	if (checkPointRepeat(mouse))  return;

	for (unsigned int i = 0; i < sVert.size(); i++) {
		QPointF& readyPoint = sVert[i];
		if (QLineF(readyPoint, mouse).length() < 0.02f / gl->viewScale.value()) {
			if (!index.contains(i)) {
				index.append(i);
				points.append(readyPoint);
			}
			return;
		}
	}

	index.append(-1);
	points.append(mouse);
}

void AddChTexPoly::draw(QPainter& painter)
{
	for (QPointF selectPoint : points) {
		selectPoint = gl->mapViewProjMatrix(selectPoint);
		painter.setPen(QPen(Qt::black, 8));
		painter.drawPoint(selectPoint);
		painter.setPen(QPen(Qt::red, 6));
		painter.drawPoint(selectPoint);
	}
}

bool AddChTexPoly::checkPointRepeat(const QPointF& point)
{
	for (QPointF& readyPoint : points) {
		if (readyPoint == point) return true;
	}
	return false;
}