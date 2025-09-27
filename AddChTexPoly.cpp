#include "AddChTexPoly.h"

void AddChTexPoly::escape()
{
	if (index.size() == 0) return;

	index.removeLast();
	points.removeLast();
}

void AddChTexPoly::enter(int currentVector)
{
	if (index.size() < 3) return;

	for (int i = 0; i < index.size(); i++) {
		if (index[i] < 0) {
			index[i] = (unsigned int)glVert[currentVector].size() / 2;
			glVert[currentVector].push_back(points[i].x());
			glVert[currentVector].push_back(points[i].y());
		}
	}

	int last2point[2] = { 0,1 };
	int front = 2;
	int back = index.size() - 1;
	bool isBack = true;
	for (int i = 2; i < index.size(); i++) {
		int v[3] = { last2point[0], last2point[1], isBack ? back : front };
		addTriangle(v, currentVector);
		last2point[0] = v[1];
		last2point[1] = v[2];
		if(isBack) back--;
		else front++;
		isBack = !isBack;
	}

	index.clear();
	points.clear();
}

void AddChTexPoly::deleteElement(int currentVector)
{
	index.clear();
	points.clear();
}

void AddChTexPoly::clickPos(const QPointF& mouse, float viewScale, int currentVector)
{
	if (checkPointRepeat(mouse))  return;

	for (unsigned int i = 0; i < glVert[currentVector].size() / 2; i++) {
		QPointF readyPoint(glVert[currentVector][i + i], glVert[currentVector][i + i + 1]);
		if (QLineF(readyPoint, mouse).length() < 0.02f / viewScale && !index.contains(i)) {
			index.append(i);
			points.append(readyPoint);
			return;
		}
	}

	index.append(-1);
	points.append(mouse);
}

bool AddChTexPoly::checkPointRepeat(const QPointF& point)
{
	for (QPointF& readyPoint : points) {
		if (readyPoint == point) return true;
	}
	return false;
}

void AddChTexPoly::addTriangle(int v[3], int currentVector)
{
	for (int i = 0; i < 3; i++) {
		glIndex[currentVector].push_back(index[v[i]]);
	}
}