#include "AddTriangle.h"

bool AddTriangle::addIndex(unsigned int i)
{
	if(numInd == 0) firstIndex = i;
	else if (numInd == 1) {
		if (firstIndex == i) return false;
		secondIndex = i;
	}
	numInd++;
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
	numVert++;
}