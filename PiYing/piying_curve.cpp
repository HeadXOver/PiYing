#include "piying_curve.h"

#include <qpolygon>

piying::Curve::Curve(const QPolygonF& poly) :
	points(poly.begin(), poly.end())
{
	if (points.size() >= 707) {
		vari_get = &Curve::get_position_poly;
	}
	else {
		vari_get = &Curve::get_position_insert;
	}
}

QPointF piying::Curve::get_position_poly(int value) const noexcept
{
	if (value >= 1000) return points.back();

	return points[value * (points.size() - 1) / 1000];
}

QPointF piying::Curve::get_position_insert(int value) const noexcept
{
	float indexF = value * (points.size() - 1) / 1000.f;
	int index = static_cast<int>(indexF);
	indexF -= index;
	QPointF res1 = points[index];
	QPointF res2 = points[index + 1];

	return indexF * res2 + (1 - indexF) * res1;
}
