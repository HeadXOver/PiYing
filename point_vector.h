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
	void clear();
	void resize(int size);
	void remove_last();
	void set_point(int i, float x, float y);
	void set_point(int i, const QPointF& p);
	void copy_to_vector(std::vector<float>& other);
	void copy_from_vector(std::vector<float>& other);
	
	bool contain(const QPointF& p) const;

	std::vector<float>& get_vector() { return points; }

	const QPointF operator[](int i) const;

	void operator+=(const PointVector& other);

private:
	std::vector<float> points;
};

