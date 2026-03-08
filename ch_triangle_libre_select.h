#pragma once

#include <memory>
#include <qpolygonf>

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
