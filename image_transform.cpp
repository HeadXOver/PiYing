#include "image_transform.h"

#include <qmatrix4x4>

ImageTransform::ImageTransform()
{
    trans_ = new QMatrix4x4();
    rot_ = new QMatrix4x4();
    scale_ = new QMatrix4x4();
}

ImageTransform::~ImageTransform()
{
    delete trans_;
    delete rot_;
    delete scale_;
}

void ImageTransform::operator=(const ImageTransform& other)
{
    *trans_ = other.get_trans();
    *rot_ = other.get_rot();
    *scale_ = other.get_scale();
}

QMatrix4x4 ImageTransform::getMatrix() const {
    return (*trans_) * (*rot_) * (*scale_);
}
QMatrix4x4 ImageTransform::getMatrixInvert() const 
{ 
    return scale_->inverted() * rot_->inverted() * trans_->inverted();
}

float ImageTransform::get_trans_x() const
{
    return (*trans_)(0, 3);
}

float ImageTransform::get_trans_y() const
{
    return (*trans_)(1, 3);
}

float ImageTransform::get_rot_degree() const
{
    return qAtan2((*rot_)(1, 0), (*rot_)(0, 0)) * 180.f / 3.141593f;
}

float ImageTransform::get_scale_x() const
{
    return sqrt((*scale_)(0, 0) * (*scale_)(0, 0) + (*scale_)(1, 0) * (*scale_)(1, 0));
}

float ImageTransform::get_scale_y() const
{
    return sqrt((*scale_)(0, 1) * (*scale_)(0, 1) + (*scale_)(1, 1) * (*scale_)(1, 1));
}

void ImageTransform::reset() {
     trans_->setToIdentity();
     rot_->setToIdentity();
     scale_->setToIdentity();
}

void ImageTransform::set_trans(const QMatrix4x4& m)
{
    *trans_ = m;
}

void ImageTransform::set_rot(const QMatrix4x4& m)
{
    *rot_ = m;
}

void ImageTransform::set_scale(const QMatrix4x4& m)
{
    *scale_ = m;
}

QMatrix4x4 ImageTransform::get_trans() const
{
    return *trans_;
}

QMatrix4x4 ImageTransform::get_rot() const
{
    return *rot_;
}

QMatrix4x4 ImageTransform::get_scale() const
{
    return *scale_;
}

QMatrix4x4* ImageTransform::trans() const
{
    return trans_;
}

QMatrix4x4* ImageTransform::rot() const
{
    return rot_;
}

QMatrix4x4* ImageTransform::scale() const
{
    return scale_;
}
