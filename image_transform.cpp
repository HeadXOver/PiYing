#include "image_transform.h"

#include <qmatrix4x4>

namespace {
    constexpr float DEGREE_TO_RADIAN = 3.1415927f / 180.0f;

    float degree_to_radian(float degree) noexcept {
        return degree * DEGREE_TO_RADIAN;
    }
}

void ImageTransform::reset() noexcept
{
    _transform.setToIdentity();
    _transform_invert.setToIdentity();

    _trans_x = 0.0f;
    _trans_y = 0.0f;
    _rot_radian = 0.0f;
    _scale_x = 1.0f;
    _scale_y = 1.0f;
}

void ImageTransform::set_trans(float x, float y) noexcept
{
    _trans_x = x;
    _trans_y = y;

    _transform(0, 3) = x;
    _transform(1, 3) = y;

    update_matrix_invert();
}

void ImageTransform::set_rot_radian(float degree) noexcept
{
    _rot_radian = degree;

    update_2_matrix();
}

void ImageTransform::set_scale(float x, float y) noexcept
{
    _scale_x = x;
    _scale_y = y;
        
    update_2_matrix();
}

void ImageTransform::set_scale(float s) noexcept
{
    _scale_x = s;
    _scale_y = s;

    update_2_matrix();
}

void ImageTransform::add_trans(float x, float y) noexcept
{
    _trans_x += x;
    _trans_y += y;

    _transform(0, 3) = _trans_x;
    _transform(1, 3) = _trans_y;

    update_matrix_invert();
}

void ImageTransform::add_rot_radian(float degree) noexcept
{
    _rot_radian += degree;

    update_2_matrix();
}

void ImageTransform::add_scale(float x, float y) noexcept
{
    _scale_x *= x;
    _scale_y *= y;

    _transform(0, 0) *= x;
    _transform(1, 0) *= x;
    _transform(0, 1) *= y;
    _transform(1, 1) *= y;

    _transform_invert(0, 0) /= x;
    _transform_invert(0, 1) /= x;
    _transform_invert(0, 3) /= x;
    _transform_invert(1, 0) /= y;
    _transform_invert(1, 1) /= y;
    _transform_invert(1, 3) /= y;
}

QMatrix4x4 ImageTransform::trans() const noexcept
{
    const float scaleMatrix[16] = {
        1.0f, 0.0f, 0.0f, _trans_x,
        0.0f, 1.0f, 0.0f, _trans_y,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    return QMatrix4x4(scaleMatrix);
}

QMatrix4x4 ImageTransform::rot() const noexcept
{
    const float cosValue = cos(_rot_radian);
    const float sinValue = sin(_rot_radian);

    const float scaleMatrix[16] = {
        cosValue, -sinValue, 0.0f, 0.0f,
        sinValue, cosValue, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    return QMatrix4x4(scaleMatrix);
}

QMatrix4x4 ImageTransform::scale() const noexcept
{
    const float scaleMatrix[16] = {
        _scale_x, 0.0f, 0.0f, 0.0f,
        0.0f, _scale_y, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    return QMatrix4x4(scaleMatrix);
}

void ImageTransform::update_2_matrix() noexcept
{
    const float cosValue = cos(_rot_radian);
    const float sinValue = sin(_rot_radian);

    const float newMatrix[16] = {
        cosValue * _scale_x, -sinValue * _scale_y, 0.0f, _trans_x,
        sinValue * _scale_x, cosValue * _scale_y, 0.0f, _trans_y,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    _transform = QMatrix4x4(newMatrix);

    const float newMatrixInvert[16] = {
        cosValue / _scale_x, sinValue / _scale_x, 0.0f, -(_trans_x * cosValue + _trans_y * sinValue) / _scale_x,
        -sinValue / _scale_y, cosValue / _scale_y, 0.0f, (_trans_x * sinValue - _trans_y * cosValue) / _scale_y,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    _transform_invert = QMatrix4x4(newMatrixInvert);
}

void ImageTransform::update_matrix_invert() noexcept
{
    const float cosValue = cos(_rot_radian);
    const float sinValue = sin(_rot_radian);

    const float newMatrixInvert[16] = {
        cosValue / _scale_x, sinValue / _scale_x, 0.0f, -(_trans_x * cosValue + _trans_y * sinValue) / _scale_x,
        -sinValue / _scale_y, cosValue / _scale_y, 0.0f, (_trans_x * sinValue - _trans_y * cosValue) / _scale_y,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    _transform_invert = QMatrix4x4(newMatrixInvert);
}
