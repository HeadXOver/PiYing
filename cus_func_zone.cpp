#include "cus_func_zone.h"

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
