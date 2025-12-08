#pragma once

class QPointF;
class QRectF;

bool isInRect(const QPointF& mouse, const QPointF& pos, int zone);
bool isInRect(const QPointF& mouse, const QPointF& pos, int width, int height);

bool isPointInTriangle(const QPointF& point, const QPointF* const triangle);

bool isRectIntersectTriangle(const QRectF& rect, const QPointF* const triangle);
