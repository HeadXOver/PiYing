#pragma once

#include <memory>
#include <qpointf>

#include "ch_element_tool_behavior.h"

class ChElementSelect;

class ChElementRectSelect final
{
public:
	ChElementRectSelect();
	~ChElementRectSelect() = default;

	void draw();
	void delete_element();
	void click(const QPointF& mouse);
	void escape();
	void mouse_move(const QPointF& mouse);
	void release(const QPointF& mouse);

private:
	std::unique_ptr<ChElementSelect> chElementSelect;
	const bool edit_skelen{ false };
	QPointF rect;
	bool isDraw = false;
};
 
