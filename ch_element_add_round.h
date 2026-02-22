#pragma once

#include "ch_element_tool_behavior.h"

#include <memory>
#include <qpointf>

class ChElementAddRound final 
{
public:
	void click(const QPointF& mouse);
	void mouse_move(const QPointF& mouse);
	void release(const QPointF& mouse);
	void draw();

private:
	void addRoundPoly(const int edgeCount);

private:
	QPointF center;
	QPointF gl_center;
	QPointF current_cursor;
		
	bool isPress{ false };

	int radius{ 0 };
	int init_angle{ 0 };
};

////////////////////////////////////////////////
 
PIYING_CLICK_BEHAVIOR(AddRoundClick, ChElementAddRound)
PIYING_MOVEMOUSE_BEHAVIOR(AddRoundMove, ChElementAddRound)
PIYING_RELEASE_BEHAVIOR(AddRoundRelease, ChElementAddRound)
PIYING_DRAW_BEHAVIOR(AddRoundDraw, ChElementAddRound)