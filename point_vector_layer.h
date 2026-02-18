#pragma once

class PointVector;
class QPointF;

class PointVectorLayer
{
public:
	PointVectorLayer(PointVector& point_vector);

	const QPointF operator[](size_t index) const;
	const QPointF operator()(size_t index) const;

	const QPointF get(size_t index, bool isSkelen) const;

	void push_back(const QPointF& point);
	void push_back(const QPointF& tex, const QPointF& ske);
	void set_point(bool first, int index, const QPointF& point);
	void copy_from_to(int from, int to);
	void resize(int size);
	void clear() noexcept;

	size_t size() const noexcept;

private:
	PointVector& point_vector;
};

