#pragma once

#include <vector>

class FloatVectorLayer
{
public:
	size_t size() const;

	float* data() const;

private:
	std::vector<float>& layer;
	int space_count;
	int* space;
	int* count;
};

