#pragma once

#include <memory>
#include <qpointf>

class ChElementSelect;

class PartVertRectSelect final
{
public:
	PartVertRectSelect();
	~PartVertRectSelect() = default;

	void draw();
	void delete_element();
	void click(const QPointF& mouse);
	void escape();
	void mouse_move(const QPointF& mouse);
	void release(const QPointF& mouse);

private:
};
