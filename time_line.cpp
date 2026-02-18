#include "time_line.h"

#include "scale_trans.h"

#include <qmatrix4x4>

Timeline::Timeline(float lenth) :_lenth(lenth), _back(lenth)
{
}

QVector4D Timeline::get_transform(int line, const ScaleTrans* scaleTrans) const
{
	float unit_lenth = 0.3f * _lenth;
	return QVector4D(
		unit_lenth * scaleTrans->scale_lenth,
		0.2f, 
		(unit_lenth - 1.f + 0.3f * _front) * scaleTrans->scale_lenth + scaleTrans->trans_x,
		0.8f - 0.4f * line + scaleTrans->trans_y
	);
}
