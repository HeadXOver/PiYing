#include "scale_trans.h"

void ScaleTrans::operator=(const ScaleTrans& other)
{
    scale_lenth = other.scale_lenth;
    scale_height = other.scale_height;
    trans_x = other.trans_x;
    trans_y = other.trans_y;
}
