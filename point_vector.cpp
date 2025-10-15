#include "point_vector.h"

#include <qpointf>

void PointVector::push_back(const QPointF& p)
{
	points.push_back(p.x()); points.push_back(p.y());
}

QPointF PointVector::operator[](int i) const {
	return QPointF(points[i + i], points[i + i + 1]);
}
