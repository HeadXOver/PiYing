#include "point_vector.h"

#include <qpointf>

void PointVector::push_back(float x, float y)
{
	points.push_back(x); 
	points.push_back(y);

	hash_point_group.push_back(-1);
}

void PointVector::push_back(const QPointF& p)
{
	push_back(p.x(), p.y());
}

void PointVector::clear()
{
	points.clear(); 
	hash_point_group.clear();
}

void PointVector::resize(int size)
{
	points.resize(size + size); 
	hash_point_group.resize(size);
}

void PointVector::remove_last()
{
	if (points.empty()) return;
	points.pop_back();
	points.pop_back();
	hash_point_group.pop_back();
}

void PointVector::set_point(int i, float x, float y)
{
	const int i2 = i + i;
	points[i2] = x;
	points[i2 + 1] = y;
}

void PointVector::set_point(int i, const QPointF& p)
{
	const int i2 = i + i;
	points[i2] = p.x();
	points[i2 + 1] = p.y();
}

void PointVector::set_hash_point_group(int vert, int group)
{
	hash_point_group[vert] = group;
}

int PointVector::get_group(int i) const
{
	return hash_point_group[i];
}

bool PointVector::contain(const QPointF& p) const
{
	for (int i = 0; i < points.size(); i += 2) {
		if (points[i] != p.x() || points[i + 1] != p.y()) continue;
		return true;
	}
	return false;
}

const QPointF PointVector::operator[](int i) const 
{
	const int i2 = i + i;
	return QPointF(points[i2], points[i2 + 1]);
}
