#include "point_vector.h"

#include <qpointf>

PointVectorLayer::PointVectorLayer(const PointVectorLayer& vector1, const PointVectorLayer& vector2)
{
	points.reserve(vector1.points.size() + vector2.points.size());

	points.insert(points.end(), vector1.points.begin(), vector1.points.end());
	points.insert(points.end(), vector2.points.begin(), vector2.points.end());
}

PointVectorLayer::PointVectorLayer(const PointVectorLayer& other)
{
	points = other.points;
}

PointVectorLayer& PointVectorLayer::operator=(const PointVectorLayer& other)
{
	points = other.points;
	return *this;
}

PointVectorLayer& PointVectorLayer::operator+=(const PointVectorLayer& other)
{
	points.insert(points.end(), other.points.begin(), other.points.end());
	return *this;
}

const QPointF PointVectorLayer::operator[](size_t index) const
{
	return PointVector::operator[](index + index);
}

const QPointF PointVectorLayer::operator()(size_t index) const
{
	return PointVector::operator[](index + index + 1);
}

const QPointF PointVectorLayer::get(size_t index, bool isSkelen) const
{
	return PointVector::operator[](index + index + (isSkelen ? 0 : 1));
}

void PointVectorLayer::push_back(const QPointF& point)
{
	PointVector::push_back(point);
	PointVector::push_back(point);
}

void PointVectorLayer::push_back(const QPointF& tex, const QPointF& ske)
{
	PointVector::push_back(ske);
	PointVector::push_back(tex);
}

void PointVectorLayer::set_point(bool edit_skelon, int index, const QPointF& point)
{
	int index_ = edit_skelon ? index + index : index + index + 1;
	PointVector::set_point(index_, point);
}

void PointVectorLayer::copy_from_to(int from, int to)
{
	PointVector::set_point(to + to, PointVector::operator[](from + from));
	PointVector::set_point(to + to + 1, PointVector::operator[](from + from + 1));
}

void PointVectorLayer::clear() noexcept
{
	PointVector::clear();
}

void PointVectorLayer::skelen_add_move(const QPointF& toMove)
{
	for (int i = 0; i < points.size(); i += 4) {
		points[i] += toMove.x();
		points[i + 1] += toMove.y();
	}
}

void PointVectorLayer::skelen_add_move(const QPointF& toMove, const PointVectorLayer& other)
{
	for (int i = 0; i < points.size(); i += 4) {
		points[i] = other.points[i] + toMove.x();
		points[i + 1] = other.points[i + 1] + toMove.y();
	}
}

size_t PointVectorLayer::element_size() const noexcept
{
	return PointVector::point_size() / 2;
}

void PointVectorLayer::resize(int size) {
	PointVector::resize(size * 2);
}
