#include "point_vector_layer.h"

#include "point_vector.h"
#include <qpointf>

PointVectorLayer::PointVectorLayer(PointVector& pointVector) : point_vector(pointVector)
{
}

const QPointF PointVectorLayer::operator[](int index) const 
{
	return point_vector[index + index];
}

const QPointF PointVectorLayer::operator()(int index) const
{
	return point_vector[index + index + 1];
}

const QPointF PointVectorLayer::get(int index, bool isSkelen) const
{
	return point_vector[index + index + (isSkelen ? 0 : 1)];
}

void PointVectorLayer::push_back(const QPointF& point) 
{
	point_vector.push_back(point);
	point_vector.push_back(point);
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

size_t PointVectorLayer::size() const 
{
	return point_vector.size() / 2;
}

void PointVectorLayer::clear() 
{
	point_vector.clear();
}

void PointVectorLayer::resize(int size) {
	point_vector.resize(size * 2);
}

int PointVectorLayer::get_group(int i) const
{
	return point_vector.get_group(i);
}
