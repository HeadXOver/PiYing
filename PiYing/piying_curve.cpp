#include "piying_curve.h"

#include <qpolygon>

piying::Curve::Curve(const QPolygonF& poly) :
	points(poly.begin(), poly.end())
{
}

QPointF piying::Curve::get_positon(int value) const noexcept
{
	if (value >= 1000) return points.back();

	if (points.size() >= 707) return points[value * (points.size() - 1) / 1000];

	float indexF = value * (points.size() - 1) / 1000.f;
	int index = static_cast<int>(indexF);
	indexF -= index;
	QPointF res1 = points[index];
	QPointF res2 = points[index + 1];

	return indexF * res2 + (1 - indexF) * res1;
}
