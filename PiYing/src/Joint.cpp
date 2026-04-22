#include "Joint.h"

#include <qpointf>

void Joint::update_matrix() noexcept
{
    localTransform.setToIdentity();
    localTransform.translate(trans_x + center_x, trans_y + center_y);
    localTransform.rotate(rot, 0, 0, 1);
    localTransform.scale(scale_x, scale_y);
    localTransform.translate(-center_x, -center_y);
}

void Joint::set_center(float x, float y) noexcept
{
    center_x = x;
    center_y = y;
    update_matrix();
}

void Joint::set_center(const QPointF& center) noexcept
{
    center_x = center.x();
    center_y = center.y();
    update_matrix();
}

QPointF Joint::get_center() const noexcept
{
    return QPointF(center_x, center_y);
}
