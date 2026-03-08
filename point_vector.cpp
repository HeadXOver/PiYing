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

void PointVector::resize(int size)
{
	points.resize(size + size); 
}

void PointVector::remove_last() noexcept
{
	if (points.empty()) return;
	points.pop_back();
	points.pop_back();
}

void PointVector::set_point(size_t i, float x, float y)
{
	const size_t i2 = i + i;
	points[i2] = x;
	points[i2 + 1] = y;
}

void PointVector::set_point(size_t i, const QPointF& p)
{
	const size_t i2 = i + i;
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

bool PointVector::contain(const QPointF& p) const noexcept
{
	for (size_t i = 0; i < points.size(); i += 2) {
		if (points[i] != p.x() || points[i + 1] != p.y()) continue;
		return true;
	}
	return false;
}

const QPointF PointVector::operator[](size_t i) const 
{
	const size_t i2 = i + i;
	return QPointF(points[i2], points[i2 + 1]);
}

///////////////////////////////////////////////////////////////////////////////////

PointVectorLayer::PointVectorLayer(PointVector& pointVector) : point_vector(pointVector)
{
}

const QPointF PointVectorLayer::operator[](size_t index) const
{
	return point_vector[index + index];
}

const QPointF PointVectorLayer::operator()(size_t index) const
{
	return point_vector[index + index + 1];
}

const QPointF PointVectorLayer::get(size_t index, bool isSkelen) const
{
	return point_vector[index + index + (isSkelen ? 0 : 1)];
}

void PointVectorLayer::push_back(const QPointF& point)
{
	point_vector.push_back(point);
	point_vector.push_back(point);
}

void PointVectorLayer::push_back(const QPointF& tex, const QPointF& ske)
{
	point_vector.push_back(ske);
	point_vector.push_back(tex);
}

void PointVectorLayer::set_point(bool edit_skelon, int index, const QPointF& point)
{
	int index_ = edit_skelon ? index + index : index + index + 1;
	point_vector.set_point(index_, point);
}

void PointVectorLayer::copy_from_to(int from, int to)
{
	point_vector.set_point(to + to, point_vector[from + from]);
	point_vector.set_point(to + to + 1, point_vector[from + from + 1]);
}

void PointVectorLayer::clear() noexcept
{
	point_vector.clear();
}

size_t PointVectorLayer::size() const noexcept
{
	return point_vector.point_size() / 2;
}

void PointVectorLayer::resize(int size) {
	point_vector.resize(size * 2);
}
