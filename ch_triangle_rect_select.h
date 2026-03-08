#pragma once

#include <memory>
#include <qpointf>

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

