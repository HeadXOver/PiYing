#include "SelectedPoints.h"

#include "point_vector_layer.h"

const QPointF& SelectedPoints::getVert(int index) const
{
	return lastVertPos[index];
}

void SelectedPoints::append(unsigned int ind)
{
	selectedIndex.append(ind);
	lastVertPos.append(QPointF());
}

void SelectedPoints::affirmVert(bool edit_skelen)
{
	for (int i = 0; i < selectedIndex.size(); i++) {
		lastVertPos[i] = sVert.get(selectedIndex[i], edit_skelen);
	}
}
