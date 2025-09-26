#pragma once

#include "ChElementSelect.h"

class AddTriangle : public ChElementSelect {
public:
	AddTriangle(std::vector<unsigned int>& ind, std::vector<float>& v) :ChElementSelect(ind, v) {}

private:
	QPointF first;
	QPointF second;

	First2VertState state;
};