#include "AddTriangle.h"
#include "piYingGL.h"

#include <qpainter>
#include <qpen>

bool AddTriangle::addIndex(unsigned int i)
{
	if(numInd == 0) firstIndex = i;
	else if (numInd == 1) {
		if (firstIndex == i) return false;
		secondIndex = i;
	}
	numInd++;
	vertThenInd = true;
	return true;
}

bool AddTriangle::checkPointRepeat(const QPointF& point)
{
	if (numVert == 0) return false;
	if (numVert == 1) {
		if(first == point) return true;
		return false;
	}
	if (first == point || second == point) return true;
	return false;
}

void AddTriangle::addVert(const QPointF& point)
{
	if (numVert == 0) first = point;
	else if (numVert == 1) second = point;
	else return;
	vertThenInd = false;
	numVert++;
}

void AddTriangle::escape()
{
	if (numVert == 0 && numInd == 0) return;
	if (numVert == 0 && numInd == 1 || numVert == 1 && numInd == 0) {
		numInd = 0;
		numVert = 0;
	}
	else if (numInd == 2) numInd--;
	else if (numVert == 2) numVert--;
	else if (vertThenInd) numInd--;
	else numVert--;
}

void AddTriangle::deleteElement()
{
	numInd = 0;
	numVert = 0;
}

void AddTriangle::addChVert(const QPointF& point)
{
	glIndex.push_back((unsigned int)sVert.size());
	addPointToVert(point);
}

void AddTriangle::clickPos(const QPointF& mouseOri)
{
	QPointF mouse = gl->getViewProjMatrixInvert().map(gl->mapToGL(mouseOri));

	if (checkPointRepeat(mouse))  return;

	int indRepeat = -1;
	for (unsigned int i = 0; i < sVert.size(); i++) {
		if (QLineF(sVert[i], mouse).length() < 0.02f / gl->viewScale.value()) {
			indRepeat = i;
			break;
		}
	}

	if (numInd + numVert <= 1) {
		if (indRepeat < 0) addVert(mouse); 
		else addIndex(indRepeat);
	}
	else if (numVert == 2) {
		numInd = 0;
		numVert = 0;
		if (indRepeat >= 0) glIndex.push_back(indRepeat);
		else  addChVert(mouse);
		addChVert(first);
		addChVert(second);
	}
	else if (numInd == 2) {
		if (indRepeat >= 0) {
			if (indRepeat == firstIndex && indRepeat == secondIndex) return;
			for (int j = 0; j < glIndex.size(); j += 3) {
				unsigned int x[3] = { glIndex[j + 0], glIndex[j + 1], glIndex[j + 2] };
				unsigned int y[3] = { (unsigned int)indRepeat, firstIndex, secondIndex };
				std::sort(x, x + 3);
				std::sort(y, y + 3);
				if (x[0] == y[0] && x[1] == y[1] && x[2] == y[2])
					return;
			}

			numInd = 0;
			numVert = 0;
			glIndex.push_back(indRepeat);
		}
		else {
			numInd = 0;
			numVert = 0;
			addChVert(mouse);
		}
		glIndex.push_back(firstIndex);
		glIndex.push_back(secondIndex);
	}
	else if (numInd == 1 && numVert == 1) {
		if (indRepeat >= 0) {
			if (firstIndex != indRepeat) {
				numInd = 0;
				numVert = 0;
				glIndex.push_back((unsigned int)indRepeat);
				glIndex.push_back(firstIndex);
				addChVert(first);
			}
		}
		else {
			numInd = 0;
			numVert = 0;
			glIndex.push_back(firstIndex);
			addChVert(mouse);
			addChVert(first);
		}
	}
}

void AddTriangle::draw(QPainter& painter)
{
	if (numInd == 0 && numVert == 0) return;
	std::vector<QPointF> toDraw;
	if (numInd == 1 && numVert == 0) {
		toDraw.push_back(sVert[firstIndex]);
	}
	else if (numInd == 0 && numVert == 1) toDraw.push_back(first);
	else if (numVert == 2) {
		toDraw.push_back(first);
		toDraw.push_back(second);
	}
	else if (numInd == 2) {
		toDraw.push_back(sVert[firstIndex]);
		toDraw.push_back(sVert[secondIndex]);
	}
	else if (numInd == 1 && numVert == 1) {
		toDraw.push_back(first);
		toDraw.push_back(sVert[firstIndex]);
	}

	for (QPointF& p : toDraw) {
		p = gl->mapViewProjMatrix(p);
		painter.setPen(QPen(Qt::black, 8));
		painter.drawPoint(p);
		painter.setPen(QPen(Qt::red, 6));
		painter.drawPoint(p);
	}
}
