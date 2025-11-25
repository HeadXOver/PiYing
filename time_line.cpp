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
	return QVector4D(QVector4D(0.3f * _lenth, 0.2f, -0.7f + 0.3f * _front, 0.8f - 0.4f * line));
}
