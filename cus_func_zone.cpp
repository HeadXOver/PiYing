#include "cus_func_zone.h"

#include "cus_func_point2d.h"

#include <QRectF>

bool isInRect(const QPointF& mouse, const QPointF& pos, int zone)
{
    QRectF centerRect(pos.x() - zone / 2, pos.y() - zone / 2, zone, zone);
    return centerRect.contains(mouse);
}

bool isInRect(const QPointF& mouse, const QPointF& pos, int width, int height)
{
    QRectF centerRect(pos.x() - width / 2, pos.y() - height / 2, width, height);
    return  centerRect.contains(mouse);
}

bool isPointInTriangle(const QPointF& point, const QPointF* const triangle)
{
    double d1 = crossProduct(point, triangle[0], triangle[1]);
    double d2 = crossProduct(point, triangle[1], triangle[2]);
    double d3 = crossProduct(point, triangle[2], triangle[0]);

    bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    // 如果三个叉积同号（全正或全负），则点在三角形内（或在边上）
    return !(hasNeg && hasPos);
}

bool isRectIntersectTriangle(const QRectF& rect, const QPointF* const triangle)
{
    for (int i = 0; i < 3; ++i) {
        if (rect.contains(triangle[i])) return true;
    }

    QPointF rectVertices[4] = { rect.topLeft(), rect.topRight(), rect.bottomLeft(), rect.bottomRight() };

    for (int i = 0; i < 4; ++i) {
        if (isPointInTriangle(rectVertices[i], triangle)) return true;
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (isCross(triangle[i], triangle[(i + 1) % 3], rectVertices[j], rectVertices[(j + 1) % 4])) return true;
        }
    }

    return false;
}

bool isInsideSquare(const QPointF& point, float side)
{
    return (point.x() >= -side / 2.f && point.x() <= side / 2.f && point.y() >= -side / 2.f && point.y() <= side / 2.f);
}
