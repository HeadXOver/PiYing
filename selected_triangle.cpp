#include "selected_triangle.h"

#include "point_vector_layer.h"

namespace {
	bool is_same_triangle(const unsigned int* a, const unsigned int* b) 
	{
		for (int i = 0; i < 3; ++i) {
			bool found = false;
			for (int j = 0; j < 3; ++j) {
				if (a[i] == b[j]) {
					found = true;
					break;
				}
			}
			if (!found) return false;
		}
		return true;
	}
}

const QPointF& SelectedTriangle::getVert(int index) const
{
	return lastVertPos[index];
}

bool SelectedTriangle::contains(const unsigned int* triangle) const
{
	for (int t = 0; t < selectedIndex.size(); t += 3) {
		if (is_same_triangle(triangle, &selectedIndex[t])) {
			return true;
		}
	}
	return false;
}

void SelectedTriangle::append(const unsigned int* ind)
{
	selectedIndex.append(ind[0]);
	selectedIndex.append(ind[1]);
	selectedIndex.append(ind[2]);

	lastVertPos.append(QPointF());
	lastVertPos.append(QPointF());
	lastVertPos.append(QPointF());
}

void SelectedTriangle::remove_last_triangle()
{
	selectedIndex.removeLast();
	selectedIndex.removeLast();
	selectedIndex.removeLast();
	
	lastVertPos.removeLast();
	lastVertPos.removeLast();
	lastVertPos.removeLast();
}

void SelectedTriangle::affirmVert(bool edit_skelen)
{
	for (int i = 0; i < selectedIndex.size(); i++) {
		lastVertPos[i] = sVert.get(selectedIndex[i], edit_skelen);
	}
}
