#include "Joint.h"

#include <qmatrix4x4>

QMatrix4x4 Joint::get_local_transform() const noexcept
{
    QMatrix4x4 m;
    m.translate(center_x, center_y);
    m.scale(scale_x, scale_y);
    m.rotate(rot, 0, 0, 1);
    m.translate(trans_x, trans_y);
    return m;
}
