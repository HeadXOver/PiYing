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