#pragma once

#include "ch_element_tool_behavior.h"

#include <memory>
#include <qpolygon>

struct ChAddVertTrace final
{
	void click(const QPointF& mouse);
	void mouse_move(const QPointF& mouse);
	void release(const QPointF& mouse);
	void draw();

	int current_index = -1;
	bool presse_on_vert = false;
	QPolygonF polygon;
};

////////////////////////////////////////////////
 
PIYING_CLICK_BEHAVIOR(AddVertTraceClick, ChAddVertTrace)
PIYING_MOVEMOUSE_BEHAVIOR(AddVertTraceMove, ChAddVertTrace)
PIYING_RELEASE_BEHAVIOR(AddVertTraceRelease, ChAddVertTrace)
PIYING_DRAW_BEHAVIOR(AddVertTraceDraw, ChAddVertTrace)