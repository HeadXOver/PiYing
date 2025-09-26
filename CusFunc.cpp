#include "CusFunc.h"

QMatrix4x4 getTrans(const QMatrix4x4& M) {
    QMatrix4x4 trans;
    trans.translate(M(0, 3), M(1, 3));
    return trans;
}

QMatrix4x4 getRot(const QMatrix4x4& M) {
    QMatrix4x4 trans;
    trans.rotate(qAtan2(M(1, 0), M(0, 0)) * 180.f / 3.141593, 0.0f, 0.0f, 1.0f);
    return trans;
}

QMatrix4x4 getScale(const QMatrix4x4& M) {
    QMatrix4x4 trans;
    trans.scale(qSqrt(M(0, 0) * M(0, 0) + M(1, 0) * M(1, 0)));
    return trans;
}

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

QString getUniquebgName(const QListWidget* list) {
    QString s;
    bool repeat = false;
    for (int i = 1;; i++) {
        s = QString::number(i);
        repeat = false;
        for (int j = 0; j < list->count(); ++j)
            if (list->item(j)->text() == s) repeat = true;
        if (!repeat) return s;
    }
}