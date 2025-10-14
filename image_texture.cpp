#include "image_texture.h"
#include "image_transform.h"

#include <qopengltexture>

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
    transform_->trans.setToIdentity();
    transform_->trans.translate(x, y);
}

void ImageTexture::setTrans(QPointF point)
{
    transform_->trans.setToIdentity();
    transform_->trans.translate(point.x(), point.y());
}

void ImageTexture::setTrans(QMatrix4x4 point)
{
    transform_->trans = point;
}

void ImageTexture::setRot(float r)
{
    transform_->rot.setToIdentity();
    transform_->rot.rotate(r, 0.f, 0.f, 1.0f);
}

void ImageTexture::setRot(QMatrix4x4 r)
{
    transform_->rot = r;
}

void ImageTexture::setScale(float x, float y)
{
    transform_->scale.setToIdentity();
    transform_->scale.scale(x, y);
}

void ImageTexture::setScale(QPointF point)
{
    transform_->scale.setToIdentity();
    transform_->scale.scale(point.x(), point.y());
}

void ImageTexture::setScale(float s)
{
    transform_->scale.setToIdentity();
    transform_->scale.scale(s);
}

void ImageTexture::setScale(QMatrix4x4 scale)
{
    transform_->scale = scale;
}

void ImageTexture::addTrans(QPointF point)
{
    addTrans(point.x(), point.y());
}

void ImageTexture::addScale(QPointF point)
{
    transform_->scale.scale(point.x(), point.y());
}

void ImageTexture::addTrans(float x, float y)
{
    transform_->trans.translate(x, y);
}

void ImageTexture::addScale(float x, float y)
{
    transform_->scale.scale(x, y);
}

void ImageTexture::addScale(float s)
{
    transform_->scale.scale(s);
}

void ImageTexture::addRot(float r)
{
    transform_->rot.rotate(r, 0.0f, 0.0f, 1.0f);
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

void ImageTexture::copyTransformFrom(ImageTransform* transform)
{
    *transform_ = *transform;
}

ImageTransform* ImageTexture::transform() const {
    return transform_;
}