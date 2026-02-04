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
    trans_ = other.trans();
    rot_ = other.rot();
    scale_ = other.scale();
}

QMatrix4x4 ImageTransform::getMatrix() const 
{
    return trans_ * rot_ * scale_;
}

QMatrix4x4 ImageTransform::getMatrixInvert() const 
{ 
    return scale_.inverted() * rot_.inverted() * trans_.inverted();
}

float ImageTransform::get_trans_x() const
{
    return trans_(0, 3);
}

float ImageTransform::get_trans_y() const
{
    return trans_(1, 3);
}

float ImageTransform::get_rot_degree() const
{
    return qAtan2(rot_(1, 0), rot_(0, 0)) * 180.f / 3.141593f;
}

float ImageTransform::get_scale_x() const
{
    return sqrt(scale_(0, 0) * scale_(0, 0) + scale_(1, 0) * scale_(1, 0));
}

float ImageTransform::get_scale_y() const
{
    return sqrt(scale_(0, 1) * scale_(0, 1) + scale_(1, 1) * scale_(1, 1));
}

void ImageTransform::reset() 
{
     trans_.setToIdentity();
     rot_.setToIdentity();
     scale_.setToIdentity();
}

void ImageTransform::set_trans(float x, float y)
{
    trans_.setToIdentity();
    trans_.translate(x, y);
}

void ImageTransform::set_trans(const QMatrix4x4& m)
{
    trans_ = m;
}

void ImageTransform::set_rot(float degree)
{
    rot_.setToIdentity();
    rot_.rotate(degree, 0, 0, 1);
}

void ImageTransform::set_rot(const QMatrix4x4& m)
{
    rot_ = m;
}

void ImageTransform::set_scale(float x, float y)
{
    scale_.setToIdentity();
    scale_.scale(x, y);
}

void ImageTransform::set_scale(float s)
{
    scale_.setToIdentity();
    scale_.scale(s, s);
}

void ImageTransform::set_scale(const QMatrix4x4& m)
{
    scale_ = m;
}

void ImageTransform::add_trans(float x, float y)
{
    trans_.translate(x, y);
}

void ImageTransform::add_rot(float degree)
{
    rot_.rotate(degree, 0, 0, 1);
}

void ImageTransform::add_scale(float x, float y)
{
    scale_.scale(x, y);
}

const QMatrix4x4& ImageTransform::trans() const
{
    return trans_;
}

const QMatrix4x4& ImageTransform::rot() const
{
    return rot_;
}

const QMatrix4x4& ImageTransform::scale() const
{
    return scale_;
}
