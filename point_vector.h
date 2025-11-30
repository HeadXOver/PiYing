#pragma once

#include <vector>

class QPointF;

class PointVector
{
public:
	size_t float_size() const { return points.size(); }
	size_t size() const { return hash_point_group.size(); }

	float* data() { return points.data(); }

	void push_back(float x, float y);
	void push_back(const QPointF& p);
	void clear();
	void resize(int size);
	void remove_last();
	void set_point(int i, float x, float y);
	void set_point(int i, const QPointF& p);
	
	bool contain(const QPointF& p) const;

	const QPointF operator[](int i) const;

private:
	std::vector<float> points;
	std::vector<int> hash_point_group;
};

