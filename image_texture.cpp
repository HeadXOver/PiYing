#include "image_texture.h"
#include "image_transform.h"

#include <qmatrix4x4>
#include <qopengltexture>
#include <qimage>
#include <qpointf>

ImageTexture::ImageTexture(QImage& image)
{
    transform_ = std::make_unique<ImageTransform>();

    texture_ = std::make_unique<QOpenGLTexture>(image.flipped());
    texture_->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture_->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_->setWrapMode(QOpenGLTexture::Repeat);
}

ImageTexture::~ImageTexture()
{
}

void ImageTexture::setTrans(float x, float y)
{
    transform_->set_trans(x, y);
}

void ImageTexture::setTrans(const QPointF& point)
{
    transform_->set_trans(point.x(), point.y());
}

void ImageTexture::setTrans(const QMatrix4x4& point)
{
    transform_->set_trans(point);
}

void ImageTexture::setRot(float r)
{
    transform_->set_rot(r);
}

void ImageTexture::setRot(const QMatrix4x4& point)
{
    transform_->set_rot(point);
}

void ImageTexture::setScale(float x, float y)
{
    transform_->set_scale(x, y);
}

void ImageTexture::setScale(const QPointF& point)
{
    transform_->set_scale(point.x(), point.y());
}

void ImageTexture::setScale(float s)
{
    transform_->set_scale(s);
}

void ImageTexture::setScale(const QMatrix4x4& point)
{
    transform_->set_scale(point);
}

void ImageTexture::addTrans(const QPointF& point)
{
    addTrans(point.x(), point.y());
}

void ImageTexture::addScale(const QPointF& point)
{
    addScale(point.x(), point.y());
}

void ImageTexture::addTrans(float x, float y)
{
    transform_->add_trans(x, y);
}

void ImageTexture::addScale(float x, float y)
{
    transform_->add_scale(x, y);
}

void ImageTexture::addScale(float s)
{
    addScale(s, s);
}

void ImageTexture::addRot(float r)
{
    transform_->add_rot(r);
}
QMatrix4x4 ImageTexture::getMatrix() const {
    return transform_->getMatrix();
}

QMatrix4x4 ImageTexture::getMatrixInvert() const
{
    return transform_->getMatrixInvert();
}

void ImageTexture::resetTransform() {
    transform_->reset();
}

void ImageTexture::bind()
{
    texture_->bind();
}

void ImageTexture::operator=(const ImageTransform& transform) {
    *transform_ = transform;
}

const ImageTransform& ImageTexture::transform() const {
    return *transform_;
}