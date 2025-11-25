#include "time_line.h"

#include <qmatrix4x4>

Timeline::Timeline(float lenth) :_lenth(lenth), _back(lenth)
{
}

float Timeline::lenth() const
{
	return _lenth;
}

QVector4D Timeline::get_transform(int line) const
{
	float unit_lenth = 0.3f * _lenth;
	return QVector4D(QVector4D(unit_lenth, 0.2f, unit_lenth - 1.f + 0.3f * _front, 0.8f - 0.4f * line));
}
