#include "point_vector.h"

#include <qpointf>

void PointVector::push_back(const QPointF& p)
{
	points.push_back(p.x()); 
	points.push_back(p.y());
}

void PointVector::remove_last()
{
	if (!points.empty()) {
		points.pop_back();
	}
}

void PointVector::set_point(int i, float x, float y)
{
	points[i + i] = x;
	points[i + i + 1] = y;
}

void PointVector::set_point(int i, const QPointF& p)
{
	points[i + i] = p.x();
	points[i + i + 1] = p.y();
}

bool PointVector::contain(const QPointF& p) const
{
	
	for (int i = 0; i < points.size(); i += 2) {
		if (points[i] == p.x() && points[i + 1] == p.y()) {
			return true;
		}
	}
	return false;
}

const QPointF PointVector::operator[](int i) const {
	return QPointF(points[i + i], points[i + i + 1]);
}
