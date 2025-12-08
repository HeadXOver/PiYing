#include "cus_func_point2d.h"

#include <QPointF>

float angleBetweenPoint(const QPointF& p1, const QPointF& p2) {
    return atan2f(p1.y(), p1.x()) - atan2f(p2.y(), p2.x());
}

float angleBetweenPointDegree(const QPointF& p1, const QPointF& p2) {
    return (atan2f(p1.y(), p1.x()) - atan2f(p2.y(), p2.x())) * 180.f / 3.1415927f;
}

QPointF getRotatedPoint(const QPointF& p, float angle) {
    return QPointF(p.x() * cosf(angle) - p.y() * sinf(angle), p.x() * sinf(angle) + p.y() * cosf(angle));
}

QPointF getRotatedPoint(float x, float y, float angle) {
    return QPointF(x * cosf(angle) - y * sinf(angle), x * sinf(angle) + y * cosf(angle));
}

QPointF getNormal(const QPointF& a, const QPointF& b)
{
    return QPointF(a.y() - b.y(), b.x() - a.x());
}

float crossProduct(const QPointF& a, const QPointF& b, const QPointF& c)
{
    return (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
}

float dot(const QPointF& a, const QPointF& b)
{
    return a.x() * b.x() + a.y() * b.y();
}

bool isCross(const QPointF& a, const QPointF& b, const QPointF& c, const QPointF& d)
{
    QPointF normal;
    float p, vert1, vert2;

    normal = getNormal(a, b);
    p = dot(normal, a);
    vert1 = dot(normal, c);
    vert2 = dot(normal, d);

    if (p > vert1 && p > vert2) return false;
    if (p < vert1 && p < vert2) return false;

    normal = getNormal(c, d);
    p = dot(normal, c);
    vert1 = dot(normal, a);
    vert2 = dot(normal, b);

    if (p > vert1 && p > vert2) return false;
    if (p < vert1 && p < vert2) return false;
    
    return true;
}
