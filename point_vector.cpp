#include "point_vector.h"

#include <qpointf>

PointVector::PointVector(const PointVector& vector1, const PointVector& vector2)
{
	points.reserve(vector1.points.size() + vector2.points.size());

	points.insert(points.end(), vector1.points.begin(), vector1.points.end());
	points.insert(points.end(), vector2.points.begin(), vector2.points.end());
}

PointVector::PointVector(const PointVector& other)
{
	points = other.points;
}

void PointVector::push_back(float x, float y)
{
	points.push_back(x); 
	points.push_back(y);
}

void PointVector::push_back(const QPointF& p)
{
	push_back(p.x(), p.y());
}

void PointVector::clear()
{
	points.clear();
}

void PointVector::resize(int size)
{
	points.resize(size + size); 
}

void PointVector::remove_last()
{
	if (points.empty()) return;
	points.pop_back();
	points.pop_back();
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

void PointVector::copy_to_vector(std::vector<float>& other)
{
	other = points;
}

void PointVector::copy_from_vector(std::vector<float>& other)
{
	points = other;
}

void PointVector::operator+=(const PointVector& other) 
{
	points.insert(points.end(), other.points.begin(), other.points.end());
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
