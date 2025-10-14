#include "AddChTexPoly.h"

#include "glVertReference.h"
#include "piYingGL.h"

#include <qpainter>

AddChTexPoly::AddChTexPoly(GlVertReference* glReference) :glVertReference(glReference)
{
}

void AddPolyEscape::escape()
{
	if (addChTexPoly->index.size() == 0) return;

	addChTexPoly->index.removeLast();
	addChTexPoly->points.removeLast();
}

void AddPolyEnter::enter() 
{
	addChTexPoly->enter();
}

void AddChTexPoly::enter()
{
	if (index.size() < 3) return;

	for (int i = 0; i < index.size(); i++) {
		if (index[i] < 0) {
			index[i] = (int)glVertReference->sVert.size();
			glVertReference->addPointToVert(points[i]);
		}
	}

	int last2point[2] = { 0,1 };
	int front = 2;
	int back = index.size() - 1;
	bool isBack = true;
	for (int i = 2; i < index.size(); i++) {
		int v[3] = { last2point[0], last2point[1], isBack ? back : front };
		for (int j = 0; j < 3; j++) {
			glVertReference->glIndex.push_back(index[v[j]]);
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

void AddPolyDelete::deleteElement()
{
	addChTexPoly->index.clear();
	addChTexPoly->points.clear();
}

void AddPolyClick::click(const QPointF& mouse) 
{
	addChTexPoly->click(mouse);
}

void AddChTexPoly::click(const QPointF& mouseOri)
{
	QPointF mouse = glVertReference->gl->getViewProjMatrixInvert().map(glVertReference->gl->mapToGL(mouseOri));

	if (checkPointRepeat(mouse))  return;

	for (unsigned int i = 0; i < glVertReference->sVert.size(); i++) {
		QPointF& readyPoint = glVertReference->sVert[i];
		if (QLineF(readyPoint, mouse).length() < 0.02f / glVertReference->gl->viewScale.value()) {
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

void AddPolyDraw::draw(QPainter* painter)
{
	for (QPointF selectPoint : addChTexPoly->points) {
		selectPoint = addChTexPoly->glVertReference->gl->mapViewProjMatrix(selectPoint);
		painter->setPen(QPen(Qt::black, 8));
		painter->drawPoint(selectPoint);
		painter->setPen(QPen(Qt::red, 6));
		painter->drawPoint(selectPoint);
	}
}

bool AddChTexPoly::checkPointRepeat(const QPointF& point)
{
	for (QPointF& readyPoint : points) {
		if (readyPoint == point) return true;
	}
	return false;
}