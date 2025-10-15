#pragma once

class QPointF;

float angleBetweenPoint(const QPointF& p1, const QPointF& p2);
float angleBetweenPointDegree(const QPointF& p1, const QPointF& p2);

QPointF getRotatedPoint(const QPointF& p, float angle);
QPointF getRotatedPoint(float x, float y, float angle);