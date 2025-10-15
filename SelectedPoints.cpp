#include "SelectedPoints.h"

#include "point_vector.h"

const QPointF& SelectedPoints::getVert(int index) const
{
	return lastVertPos[index];
}

void SelectedPoints::append(unsigned int ind)
{
	selectedIndex.append(ind); lastVertPos.append(sVert[ind]);
}

void SelectedPoints::affirmVert()
{
	for (int i = 0; i < selectedIndex.size(); i++) {
		lastVertPos[i] = sVert[selectedIndex[i]];
	}
}
