#pragma once

#include "ChElementSelect.h"

class ChElementLibreSelect : public ChElementSelect
{
public:
	ChElementLibreSelect(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v, PiYingGL* gl) :ChElementSelect(ind, v, gl) {}

private:
	std::vector<QPointF> curve;
};

