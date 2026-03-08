#include "point_vector.h"

#include <qpointf>

PointVectorLayerToMut::PointVectorLayerToMut(PointVector& pointVector) : point_vector(pointVector)
{
}

const QPointF PointVectorLayerToMut::operator[](size_t index) const
{
	return point_vector[index + index];
}

const QPointF PointVectorLayerToMut::operator()(size_t index) const
{
	return point_vector[index + index + 1];
}

const QPointF PointVectorLayerToMut::get(size_t index, bool isSkelen) const
{
	return point_vector[index + index + (isSkelen ? 0 : 1)];
}

void PointVectorLayerToMut::push_back(const QPointF& point) 
{
	point_vector.push_back(point);
	point_vector.push_back(point);
}

void PointVectorLayerToMut::push_back(const QPointF& tex, const QPointF& ske)
{
	point_vector.push_back(ske);
	point_vector.push_back(tex);
}

void PointVectorLayerToMut::set_point(bool edit_skelon, int index, const QPointF& point)
{
	int index_ = edit_skelon ? index + index : index + index + 1;
	point_vector.set_point(index_, point);
}

void PointVectorLayerToMut::copy_from_to(int from, int to)
{
	point_vector.set_point(to + to, point_vector[from + from]);
	point_vector.set_point(to + to + 1, point_vector[from + from + 1]);
}

void PointVectorLayerToMut::clear() noexcept
{
	point_vector.clear();
}

size_t PointVectorLayerToMut::size() const noexcept 
{
	return point_vector.point_size() / 2; 
}

void PointVectorLayerToMut::resize(int size) {
	point_vector.resize(size * 2);
}
