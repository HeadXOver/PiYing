#include "cus_func_zone.h"

#include "cus_func_point2d.h"

#include <QRectF>

bool isInRect(const QPointF& mouse, const QPointF& pos, int zone)
{
    QRectF centerRect(pos.x() - zone / 2, pos.y() - zone / 2, zone, zone);
    if (centerRect.contains(mouse)) {
        return true;
    }
    return false;
}

bool isInRect(const QPointF& mouse, const QPointF& pos, int width, int height)
{
    QRectF centerRect(pos.x() - width / 2, pos.y() - height / 2, width, height);
    if (centerRect.contains(mouse)) {
        return true;
    }
    return false;
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
