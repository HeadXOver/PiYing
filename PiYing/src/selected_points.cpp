#include "selected_points.h"

#include "point_vector.h"
#include "tool_vert_select.h"

void piying::SelectedPoints::append(unsigned int ind)
{
	selectedIndex.append(ind);
	lastVertPos.append(QPointF());
}

void piying::SelectedPoints::affirmVert(const PointVectorLayer& fromLayer, bool edit_skelen)
{
	for (int i = 0; i < selectedIndex.size(); i++) {
		lastVertPos[i] = fromLayer.get(selectedIndex[i], edit_skelen);
	}
}
