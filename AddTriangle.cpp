#include "AddTriangle.h"

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
		return;
	}
	if (numInd == 2) {
		numInd--;
		return;
	}
	if (numVert == 2) {
		numVert--;
		return;
	}
	if (vertThenInd) {
		numInd--;
		return;
	}
	numVert--;
}

void AddTriangle::deleteElement(int currentVector)
{
	numInd = 0;
	numVert = 0;
}

void AddTriangle::addChVert(const QPointF& point, int currentVector)
{
	glIndex[currentVector].push_back((unsigned int)glVert[currentVector].size() / 2);
	glVert[currentVector].push_back(point.x());
	glVert[currentVector].push_back(point.y());
}

void AddTriangle::clickPos(const QPointF& mouse, float viewScale, int currentVector)
{
	if (checkPointRepeat(mouse))  return;

	int indRepeat = -1;
	for (unsigned int i = 0; i < glVert[currentVector].size() / 2; i++) {
		QPointF readyPoint(glVert[currentVector][i + i], glVert[currentVector][i + i + 1]);
		if (QLineF(readyPoint, mouse).length() < 0.02f / viewScale) {
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
		if (indRepeat >= 0) glIndex[currentVector].push_back(indRepeat);
		else  addChVert(mouse, currentVector);
		addChVert(first, currentVector);
		addChVert(second, currentVector);
	}
	else if (numInd == 2) {
		if (indRepeat >= 0) {
			if (indRepeat == firstIndex && indRepeat == secondIndex) return;
			for (int j = 0; j < glIndex[currentVector].size(); j += 3) {
				unsigned int x[3] = { glIndex[currentVector][j + 0], glIndex[currentVector][j + 1], glIndex[currentVector][j + 2] };
				unsigned int y[3] = { (unsigned int)indRepeat, firstIndex, secondIndex };
				std::sort(x, x + 3);
				std::sort(y, y + 3);
				if (x[0] == y[0] && x[1] == y[1] && x[2] == y[2])
					return;
			}

			numInd = 0;
			numVert = 0;
			glIndex[currentVector].push_back(indRepeat);
		}
		else {
			numInd = 0;
			numVert = 0;
			addChVert(mouse, currentVector);
		}
		glIndex[currentVector].push_back(firstIndex);
		glIndex[currentVector].push_back(secondIndex);
	}
	else if (numInd == 1 && numVert == 1) {
		if (indRepeat >= 0) {
			if (firstIndex != indRepeat) {
				numInd = 0;
				numVert = 0;
				glIndex[currentVector].push_back((unsigned int)indRepeat);
				glIndex[currentVector].push_back(firstIndex);
				addChVert(first, currentVector);
			}
		}
		else {
			numInd = 0;
			numVert = 0;
			glIndex[currentVector].push_back(firstIndex);
			addChVert(mouse, currentVector);
			addChVert(first, currentVector);
		}
	}
}

QList<QPointF> AddTriangle::getToDrawVert(int currentVector)
{
	if (numInd == 0 && numVert == 0) return {};
	if (numInd == 1 && numVert == 0) {
		int index = firstIndex * 2;
		return { QPointF(glVert[currentVector][index], glVert[currentVector][index + 1]) };
	}
	else if (numInd == 0 && numVert == 1) return { first };
	else if (numVert == 2) return { first, second };
	else if (numInd == 2) {
		int index = firstIndex * 2;
		QPointF selectPoint(glVert[currentVector][index], glVert[currentVector][index + 1]);
		index = secondIndex * 2;
		QPointF selectPoint2(glVert[currentVector][index], glVert[currentVector][index + 1]);
		return{ selectPoint , selectPoint2 };
	}
	else if (numInd == 1 && numVert == 1) {
		int index = firstIndex * 2;
		QPointF selectPoint(glVert[currentVector][index], glVert[currentVector][index + 1]);
		return{ first, selectPoint };
	}
	return{};
}
