#include "selected_triangle.h"

#include "point_vector_layer.h"

namespace {
	bool is_same_triangle(const unsigned int* a, const unsigned int* b) 
	{
		return (a[0] == b[0] && a[1] == b[1] && a[2] == b[2]) ||
			(a[0] == b[0] && a[1] == b[2] && a[2] == b[1]) ||
			(a[0] == b[1] && a[1] == b[0] && a[2] == b[2]) ||
			(a[0] == b[1] && a[1] == b[2] && a[2] == b[0]) ||
			(a[0] == b[2] && a[1] == b[0] && a[2] == b[1]) ||
			(a[0] == b[2] && a[1] == b[1] && a[2] == b[0]);
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

void SelectedTriangle::remove_last_triangle() noexcept
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
