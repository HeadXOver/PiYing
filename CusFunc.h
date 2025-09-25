#pragma once

#include <QMatrix4x4>
#include <QLineF>

inline QMatrix4x4 getTrans(const QMatrix4x4& M) {
    QMatrix4x4 trans;
    trans.translate(M(0, 3), M(1, 3));
    return trans;
}

inline QMatrix4x4 getRot(const QMatrix4x4& M) {
    QMatrix4x4 trans;
    trans.rotate(qAtan2(M(1, 0), M(0, 0)) * 180.f / 3.141593, 0.0f, 0.0f, 1.0f);
    return trans;
}

inline QMatrix4x4 getScale(const QMatrix4x4& M) {
    QMatrix4x4 trans;
    trans.scale(qSqrt(M(0, 0) * M(0, 0) + M(1, 0) * M(1, 0)));
    return trans;
}

inline float angleBetweenPoint(const QPointF& p1, const QPointF& p2) {
    return atan2f(p1.y(), p1.x()) - atan2f(p2.y(), p2.x());
}

inline float angleBetweenPointDegree(const QPointF& p1, const QPointF& p2) {
    return (atan2f(p1.y(), p1.x()) - atan2f(p2.y(), p2.x())) * 180.f / 3.1415927f;
}