#pragma once

#include "CusInclude.h"

class ChElementSelect
{
public:
	ChElementSelect(std::vector<unsigned int>& ind, std::vector<float>& v) :glIndex(ind), glVert(v) {}

private:
	QList<unsigned int> index;
	QList<QPointF> vert;

	std::vector<float>& glVert;
	std::vector<unsigned int>& glIndex;
};

