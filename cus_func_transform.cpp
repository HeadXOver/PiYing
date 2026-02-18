#include "cus_func_transform.h"

#include <QMatrix4x4>

QMatrix4x4 PiYingCus::getTrans(const QMatrix4x4& M) noexcept
{
    QMatrix4x4 trans;
    trans.translate(M(0, 3), M(1, 3));
    return trans;
}

QMatrix4x4 PiYingCus::getRot(const QMatrix4x4& M) 
{
    QMatrix4x4 trans;
    trans.rotate(qAtan2(M(1, 0), M(0, 0)) * 180.f / 3.141593, 0.0f, 0.0f, 1.0f);
    return trans;
}

QMatrix4x4 PiYingCus::getScale(const QMatrix4x4& M) 
{
    QMatrix4x4 trans;
    trans.scale(qSqrt(M(0, 0) * M(0, 0) + M(1, 0) * M(1, 0)));
    return trans;
}
