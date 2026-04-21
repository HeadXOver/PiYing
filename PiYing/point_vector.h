#pragma once

#include <vector>

class QPointF;

class PointVector
{
public:
	PointVector() = default;
	PointVector(const PointVector& vector1, const PointVector& vector2);
	PointVector(const PointVector& other);
	~PointVector() = default;

	size_t float_size() const noexcept { return points.size(); }
	size_t point_size() const noexcept { return points.size() / (size_t)2; }
	size_t half_point_size() const noexcept { return points.size() / (size_t)4; }

	const float* data() const noexcept { return points.data(); }

	void push_back(float x, float y);
	void push_back(const QPointF& p);
	void clear() noexcept{ points.clear(); }
	void resize(int size);
	void remove_last() noexcept;
	void set_point(size_t i, float x, float y);
	void set_point(size_t i, const QPointF& p);
	void copy_to_vector(std::vector<float>& other);
	void copy_from_vector(std::vector<float>& other);
	
	bool contain(const QPointF& p) const noexcept;

	std::vector<float>& get_vector() noexcept { return points; }

	const QPointF operator[](size_t i) const;

	void operator+=(const PointVector& other);

protected:
	std::vector<float> points;
};

////////////////////////////////////////////////////////////////////////////////////

class PointVectorLayer : private PointVector
{
public:
	PointVectorLayer() = default;
	~PointVectorLayer() = default;

	PointVectorLayer(const PointVectorLayer& vector1, const PointVectorLayer& vector2);
	PointVectorLayer(const PointVectorLayer& other);

	PointVectorLayer& operator=(const PointVectorLayer& other);
	PointVectorLayer& operator+=(const PointVectorLayer& other);

public:
	const QPointF operator[](size_t index) const;
	const QPointF operator()(size_t index) const;

	const QPointF get(size_t index, bool isSkelen) const;

	void push_back(double x, double y);
	void push_back(const QPointF& tex, const QPointF& ske);
	void set_point(bool first, int index, const QPointF& point);
	void copy_from_to(int from, int to);
	void resize(int size);
	void clear() noexcept;
	void skelen_add_move(const QPointF& toMove);
	void skelen_add_move(const QPointF& toMove, const PointVectorLayer& other);

	const float* data() const noexcept { return points.data(); }

	size_t float_size() const noexcept { return points.size(); }
	size_t element_size() const noexcept;
};