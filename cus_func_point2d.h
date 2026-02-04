#pragma once

class QPointF;

namespace PiYingCus {

	float angleBetweenPoint(const QPointF& p1, const QPointF& p2);
	float angleBetweenPointDegree(const QPointF& p1, const QPointF& p2);

	QPointF getRotatedPoint(const QPointF& p, float angle);
	QPointF getRotatedPoint(float x, float y, float angle);

	QPointF getNormal(const QPointF& a, const QPointF& b);

	float crossProduct(const QPointF& a, const QPointF& b, const QPointF& c);
	float dot(const QPointF& a, const QPointF& b);

	bool isCross(const QPointF& a, const QPointF& b, const QPointF& c, const QPointF& d);

}
