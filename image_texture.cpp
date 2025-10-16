#include "image_texture.h"
#include "image_transform.h"

#include <qmatrix4x4>
#include <qopengltexture>
#include <qimage>
#include <qpointf>

ImageTexture::ImageTexture(QImage& image)
{
    transform_ = new ImageTransform();

    texture_ = new QOpenGLTexture(image.flipped());
    texture_->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture_->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_->setWrapMode(QOpenGLTexture::Repeat);
}

ImageTexture::~ImageTexture()
{
    delete transform_;
    delete texture_;
}

void ImageTexture::setTrans(float x, float y)
{
    transform_->trans()->setToIdentity();
    transform_->trans()->translate(x, y);
}

void ImageTexture::setTrans(const QPointF& point)
{
    transform_->trans()->setToIdentity();
    transform_->trans()->translate(point.x(), point.y());
}

void ImageTexture::setTrans(const QMatrix4x4& point)
{
    transform_->set_trans(point);
}

void ImageTexture::setRot(float r)
{
    transform_->rot()->setToIdentity();
    transform_->rot()->rotate(r, 0.f, 0.f, 1.0f);
}

void ImageTexture::setRot(const QMatrix4x4& point)
{
    transform_->set_rot(point);
}

void ImageTexture::setScale(float x, float y)
{
    transform_->scale()->setToIdentity();
    transform_->scale()->scale(x, y);
}

void ImageTexture::setScale(const QPointF& point)
{
    transform_->scale()->setToIdentity();
    transform_->scale()->scale(point.x(), point.y());
}

void ImageTexture::setScale(float s)
{
    transform_->scale()->setToIdentity();
    transform_->scale()->scale(s);
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
    transform_->scale()->scale(point.x(), point.y());
}

void ImageTexture::addTrans(float x, float y)
{
    transform_->trans()->translate(x, y);
}

void ImageTexture::addScale(float x, float y)
{
    transform_->scale()->scale(x, y);
}

void ImageTexture::addScale(float s)
{
    transform_->scale()->scale(s);
}

void ImageTexture::addRot(float r)
{
    transform_->rot()->rotate(r, 0.0f, 0.0f, 1.0f);
}
QMatrix4x4 ImageTexture::getMatrix() const {
    return transform_->getMatrix();
}

QMatrix4x4 ImageTexture::getMatrixInvert() const
{
    return transform_->getMatrixInvert();
}

QOpenGLTexture* ImageTexture::texture() const
{
    return texture_;
}

void ImageTexture::resetTransform() {
    transform_->reset();
}

void ImageTexture::operator=(const ImageTransform& transform) {
    *transform_ = transform;
}

void ImageTexture::operator=(const ImageTransform* transform)
{
    *transform_ = *transform;
}

ImageTransform* ImageTexture::transform() const {
    return transform_;
}