#include "selected_points.h"

#include "point_vector.h"

namespace {
	bool is_same_triangle(std::array<unsigned int, 3> a, std::array<unsigned int, 3> b) noexcept
	{
		return (a[0] == b[0] && a[1] == b[1] && a[2] == b[2]) ||
			(a[0] == b[0] && a[1] == b[2] && a[2] == b[1]) ||
			(a[0] == b[1] && a[1] == b[0] && a[2] == b[2]) ||
			(a[0] == b[1] && a[1] == b[2] && a[2] == b[0]) ||
			(a[0] == b[2] && a[1] == b[0] && a[2] == b[1]) ||
			(a[0] == b[2] && a[1] == b[1] && a[2] == b[0]);
	}
}

bool piying::SelectedTriangle::contains(std::array<unsigned int, 3> triangle) const noexcept
{
	std::array<unsigned int, 3> selectedTriangle;
	assert(selectedIndex.size() % 3 == 0);
	for (int t = 0; t < selectedIndex.size(); t += 3) {
		selectedTriangle = { selectedIndex[t], selectedIndex[t + 1], selectedIndex[t + 2] };
		if (is_same_triangle(triangle, selectedTriangle)) {
			return true;
		}
	}
	return false;
}

void piying::SelectedTriangle::append(std::array<unsigned int, 3> ind)
{
	selectedIndex.append(ind[0]);
	selectedIndex.append(ind[1]);
	selectedIndex.append(ind[2]);

	lastVertPos.emplace_back();
	lastVertPos.emplace_back();
	lastVertPos.emplace_back();
}

void piying::SelectedTriangle::remove_last_triangle() noexcept
{
	selectedIndex.removeLast();
	selectedIndex.removeLast();
	selectedIndex.removeLast();
	
	lastVertPos.removeLast();
	lastVertPos.removeLast();
	lastVertPos.removeLast();
}
