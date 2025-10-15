#include "point_vector_layer.h"

#include "point_vector.h"
#include <qpointf>

PointVectorLayer::PointVectorLayer(PointVector& pointVector) : point_vector(pointVector)
{
}

QPointF PointVectorLayer::get_uv_point(int index) const
{
	return point_vector[index + index + 1];
}

const QPointF& PointVectorLayer::operator[](int index) const 
{
	return point_vector[index + index];
}

void PointVectorLayer::push_back(const QPointF& point) 
{
	point_vector.push_back(point);
	point_vector.push_back(point);
}

void PointVectorLayer::set_point(bool first, int index, const QPointF& point)
{
	int index_ = first ? index + index : index + index + 1;
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
