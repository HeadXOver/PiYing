#pragma once

#include <memory>
#include <qpolygon>

class ChAddVertTrace final
{
public:
	void click(const QPointF& mouse);
	void mouse_move(const QPointF& mouse);
	void release(const QPointF& mouse);
	void draw();

private:
	int current_index = -1;
	bool presse_on_vert = false;
	QPolygonF polygon;
};