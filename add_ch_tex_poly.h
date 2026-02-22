#pragma once

#include <QList>
#include <QPoint>
#include <memory>

#include "ch_element_tool_behavior.h"

/// <summary>
/// @brief 添加多边形
/// @details 点击多次后回车
/// </summary>
class AddChTexPoly final
{
public:
	void escape();
	void click(const QPointF& mouse);
	void enter();
	void delete_element();
	void draw();

private:
	bool checkPointRepeat(const QPointF& point);

	QList<QPointF> points;
	QList<int> index;
};

////////////////////////////////////////////////////////

PIYING_ESCAPE_BEHAVIOR(AddPolyEscape, AddChTexPoly)
PIYING_DELETE_BEHAVIOR(AddPolyDelete, AddChTexPoly)
PIYING_CLICK_BEHAVIOR(AddPolyClick, AddChTexPoly)
PIYING_DRAW_BEHAVIOR(AddPolyDraw, AddChTexPoly)
PIYING_ENTER_BEHAVIOR(AddPolyEnter, AddChTexPoly)

