#include "SelectedPoints.h"

void SelectedPoints::affirmVert()
{
	for (int i = 0; i < selectedIndex.size(); i++) {
		lastVertPos[i] = sVert[selectedIndex[i]];
	}
}
