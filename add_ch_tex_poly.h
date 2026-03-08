#pragma once

#include <QList>
#include <QPoint>
#include <memory>

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


