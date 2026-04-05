#pragma once

class QPointF;
class QRectF;
class QWidget;

enum class ToolHandleControlMode;

namespace piying {

	bool isInRect(const QPointF& mouse, const QPointF& pos, int zone);
	bool isInRect(const QPointF& mouse, const QPointF& pos, int width, int height);

	bool isPointInTriangle(const QPointF& point, const QPointF* const triangle)noexcept;

	bool isRectIntersectTriangle(const QRectF& rect, const QPointF* const triangle);

	bool isInsideSquare(const QPointF& point, float side = 2.0f) noexcept;

	ToolHandleControlMode controlModeByMousePos(const QPointF& mouse, const QPointF& center);

	void drawHandleZone(QWidget* painter, const QPointF& position);

}
