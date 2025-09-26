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
