#pragma once

class PointVector;
class QPointF;

class PointVectorLayer
{
public:
	PointVectorLayer(PointVector& point_vector);

	const QPointF& operator[](int index) const;
	const QPointF& operator()(int index) const;

	void push_back(const QPointF& point);
	void set_point(bool first, int index, const QPointF& point);
	void copy_from_to(int from, int to);
	void clear();
	void resize(int size);
	

	size_t size() const;

private:
	PointVector& point_vector;
};

