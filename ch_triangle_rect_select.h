#pragma once

#include <memory>
#include <qpointf>

#include "ch_element_tool_behavior.h"

class ChTriangleSelect;

class ChTriangleRectSelect final
{
public:
	ChTriangleRectSelect();
	~ChTriangleRectSelect() = default;

	void draw();
	void click(const QPointF& mouse);
	void mouse_move(const QPointF& mouse);
	void release(const QPointF& mouse);
	void delete_element();
	void escape();
	void enter();

private:
	std::unique_ptr<ChTriangleSelect> chTriangleSelect;
	const bool edit_skelen{ false };
	QPointF rect;
	bool isDraw = false;
};

///////////////////////////////////////////////////
 
PIYING_CLICK_BEHAVIOR(RectSelectTriangleClick, ChTriangleRectSelect)
PIYING_MOVEMOUSE_BEHAVIOR(RectSelectTriangleMove, ChTriangleRectSelect)
PIYING_RELEASE_BEHAVIOR(RectSelectTriangleRelease, ChTriangleRectSelect)
PIYING_ESCAPE_BEHAVIOR(RectSelectTriangleEscape, ChTriangleRectSelect)
PIYING_DELETE_BEHAVIOR(RectSelectTriangleDelete, ChTriangleRectSelect)
PIYING_DRAW_BEHAVIOR(RectSelectTriangleDraw, ChTriangleRectSelect)
PIYING_ENTER_BEHAVIOR(RectSelectTriangleEnter, ChTriangleRectSelect)

