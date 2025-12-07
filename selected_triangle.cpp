#include "selected_triangle.h"

#include "point_vector_layer.h"

const QPointF& SelectedTriangle::getVert(int index) const
{
	return lastVertPos[index];
}

void SelectedTriangle::append(unsigned int ind)
{
	selectedIndex.append(ind);
}

void SelectedTriangle::affirmVert(bool edit_skelen)
{
	lastVertPos.clear();
	lastVertPos.reserve(selectedIndex.size());
	for (int i = 0; i < selectedIndex.size(); i++) {
		lastVertPos[i] = edit_skelen ?
			sVert[selectedIndex[i]] :
			sVert(selectedIndex[i]);
	}
}
