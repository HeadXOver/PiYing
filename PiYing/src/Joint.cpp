#include "Joint.h"

#include <qmatrix4x4>

QMatrix4x4 Joint::get_local_transform() const noexcept
{
    QMatrix4x4 m;
    m.translate(trans_x + center_x, trans_y + center_y);
    m.rotate(rot, 0, 0, 1);
    m.scale(scale_x, scale_y);
    m.translate(-center_x, -center_y);
    return m;
}

void Joint::set_center(float x, float y) noexcept
{
    center_x = x;
    center_y = y;
}
