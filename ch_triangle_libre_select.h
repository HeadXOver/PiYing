#pragma once

#include <memory>
#include <qpolygonf>

#include "ch_element_tool_behavior.h"

class ChTriangleSelect;

class ChTriangleLibreSelect final
{
public:
	ChTriangleLibreSelect();
	~ChTriangleLibreSelect() = default;

	void draw();
	void click(const QPointF& mouse);
	void mouse_move(const QPointF& mouse);
	void release(const QPointF& mouse);
	void escape();
	void delete_element();
	void enter();

private:
	std::unique_ptr<ChTriangleSelect> chTriangleSelect;
	QPolygonF polygon;
	bool drawing = false;
	bool edit_skelen{ false };
	bool isPress = false;
};

/// ////////////////////////////////////////////////////////////////////////

PIYING_CLICK_BEHAVIOR(LibreSelectTriangleClick, ChTriangleLibreSelect)
PIYING_MOVEMOUSE_BEHAVIOR(LibreSelectTriangleMove, ChTriangleLibreSelect)
PIYING_RELEASE_BEHAVIOR(LibreSelectTriangleRelease, ChTriangleLibreSelect)
PIYING_ENTER_BEHAVIOR(LibreSelectTriangleEnter, ChTriangleLibreSelect)
PIYING_DRAW_BEHAVIOR(LibreSelectTriangleDraw, ChTriangleLibreSelect)
PIYING_DELETE_BEHAVIOR(LibreSelectTriangleDelete, ChTriangleLibreSelect)
PIYING_ESCAPE_BEHAVIOR(LibreSelectTriangleEscape, ChTriangleLibreSelect)
