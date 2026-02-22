#pragma once

#include <memory>
#include <qpolygonf>

#include "ch_element_tool_behavior.h"

class ChElementSelect;

class ChElementLibreSelect final
{
public:
	ChElementLibreSelect();
	~ChElementLibreSelect() = default;

	void draw();
	void click(const QPointF& mouse);
	void escape();
	void delete_element();
	void mouse_move(const QPointF& mouse);
	void release(const QPointF& mouse);
	void enter();

private:
	std::unique_ptr<ChElementSelect> chElementSelect;
	QPolygonF polygon;
	bool drawing = false;
	bool edit_skelen{ false }; 
	bool isPress = false;
};

///////////////////////////////////////////////////

PIYING_CLICK_BEHAVIOR(LibreSelectClick, ChElementLibreSelect)
PIYING_MOVEMOUSE_BEHAVIOR(LibreSelectMove, ChElementLibreSelect)
PIYING_RELEASE_BEHAVIOR(LibreSelectRelease, ChElementLibreSelect)
PIYING_ESCAPE_BEHAVIOR(LibreSelectEscape, ChElementLibreSelect)
PIYING_DELETE_BEHAVIOR(LibreSelectDelete, ChElementLibreSelect)
PIYING_ENTER_BEHAVIOR(LibreSelectEnter, ChElementLibreSelect)
PIYING_DRAW_BEHAVIOR(LibreSelectDraw, ChElementLibreSelect)