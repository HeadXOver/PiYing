#pragma once

#include "ch_element_tool_behavior.h"

#include <memory>
#include <qpointf>

/// <summary>
/// @brief 添加一个三角形
/// @details 点击三次，添加一个三角形
/// </summary>
class AddTriangle final
{
public:
	void click(const QPointF& mouse);
	void delete_element() noexcept;
	void escape() noexcept;
	void draw();

private:
	bool addVert(unsigned int i) noexcept;
	bool checkPointRepeat(const QPointF& point) const noexcept;

	void addVert(const QPointF& point) noexcept;

private:
	QPointF first;
	QPointF second;

	unsigned int firstIndex = 0;
	unsigned int secondIndex = 0;

	bool vertThenInd = false;

	int numVert = 0;
	int numInd = 0;
};

////////////////////////////////////////////////////////

PIYING_ESCAPE_BEHAVIOR(AddTriangleEscape, AddTriangle)
PIYING_DELETE_BEHAVIOR(AddTriangleDelete, AddTriangle)
PIYING_CLICK_BEHAVIOR(AddTriangleClick, AddTriangle)
PIYING_DRAW_BEHAVIOR(AddTriangleDraw, AddTriangle)
