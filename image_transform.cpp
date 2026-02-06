#include "image_transform.h"

#include <qmatrix4x4>

ImageTransform::ImageTransform()
{
}

ImageTransform::~ImageTransform()
{
}

void ImageTransform::operator=(const ImageTransform& other)
{
    _transform = other._transform;
    _transform_invert = other._transform_invert;

    _trans_x = other._trans_x;
    _trans_y = other._trans_y;
    _rot_degree = other._rot_degree;
    _scale_x = other._scale_x;
    _scale_y = other._scale_y;
}

const QMatrix4x4& ImageTransform::getMatrix() const 
{
    return _transform;
}

const QMatrix4x4& ImageTransform::getMatrixInvert() const 
{ 
    return _transform_invert;
}

float ImageTransform::get_trans_x() const
{
    return _trans_x;
}

float ImageTransform::get_trans_y() const
{
    return _trans_y;
}

float ImageTransform::get_rot_degree() const
{
    return _rot_degree;
}

float ImageTransform::get_scale_x() const
{
    return _scale_x;
}

float ImageTransform::get_scale_y() const
{
    return _scale_y;
}

void ImageTransform::reset()
{
    _transform.setToIdentity();
    _transform_invert.setToIdentity();

    _trans_x = 0.0f;
    _trans_y = 0.0f;
    _rot_degree = 0.0f;
    _scale_x = 1.0f;
    _scale_y = 1.0f;
}

void ImageTransform::set_trans(float x, float y)
{
    _trans_x = x;
    _trans_y = y;

    update_matrix();
}

void ImageTransform::set_rot(float degree)
{
    _rot_degree = degree;

    update_matrix();
}

void ImageTransform::set_scale(float x, float y)
{
    _scale_x = x;
    _scale_y = y;
        
    update_matrix();
}

void ImageTransform::set_scale(float s)
{
    _scale_x = s;
    _scale_y = s;

    update_matrix();
}

void ImageTransform::add_trans(float x, float y)
{
    _trans_x += x;
    _trans_y += y;

    update_matrix();
}

void ImageTransform::add_rot(float degree)
{
    _rot_degree += degree;

    update_matrix();
}

void ImageTransform::add_scale(float x, float y)
{
    _scale_x *= x;
    _scale_y *= y;

    update_matrix();
}

QMatrix4x4 ImageTransform::trans() const
{
    QMatrix4x4 trans;
    trans.translate(_trans_x, _trans_y);

    return trans;
}

QMatrix4x4 ImageTransform::rot() const
{
    QMatrix4x4 rot;
    rot.rotate(_rot_degree, 0.0f, 0.0f, 1.0f);

    return rot;
}

QMatrix4x4 ImageTransform::scale() const
{
    QMatrix4x4 scale;
    scale.scale(_scale_x, _scale_y);

    return scale;
}

void ImageTransform::update_matrix()
{
    _transform.setToIdentity();

    _transform.translate(_trans_x, _trans_y);
    _transform.rotate(_rot_degree, 0.0f, 0.0f, 1.0f);
    _transform.scale(_scale_x, _scale_y);

    _transform_invert.setToIdentity();

    _transform_invert.scale(1.0f / _scale_x, 1.0f / _scale_y);
    _transform_invert.rotate(-_rot_degree, 0.0f, 0.0f, 1.0f);
    _transform_invert.translate(-_trans_x, -_trans_y);
}
