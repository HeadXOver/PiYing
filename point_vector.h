#pragma once

#include <vector>

class QPointF;

class PointVector
{
public:
	size_t float_size() const { return points.size(); }

	float* data() { return points.data(); }

	void push_back(float x, float y) { points.push_back(x); points.push_back(y); }
	void push_back(const QPointF& p);
	void clear() { points.clear(); }
	void resize(int size) { points.resize(size + size); }

	QPointF operator[](int i) const;

private:
	std::vector<float> points;
};

