#include "image_texture.h"
#include "image_transform.h"

#include <qmatrix4x4>
#include <qopengltexture>
#include <qimage>
#include <qpointf>

ImageTexture::ImageTexture(const QImage& image)
{
    _transform = std::make_unique<ImageTransform>();

    _texture = std::make_unique<QOpenGLTexture>(image.flipped());
    _texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _texture->setMagnificationFilter(QOpenGLTexture::Linear);
    _texture->setWrapMode(QOpenGLTexture::ClampToEdge);
}

ImageTexture::~ImageTexture()
{
}

void ImageTexture::setTrans(float x, float y)
{
    _transform->set_trans(x, y);
}

void ImageTexture::setTrans(const QPointF& point)
{
    _transform->set_trans(point.x(), point.y());
}

void ImageTexture::setTrans(const QMatrix4x4& point)
{
    _transform->set_trans(point);
}

void ImageTexture::setRot(float r)
{
    _transform->set_rot(r);
}

void ImageTexture::setRot(const QMatrix4x4& point)
{
    _transform->set_rot(point);
}

void ImageTexture::setScale(float x, float y)
{
    _transform->set_scale(x, y);
}

void ImageTexture::setScale(const QPointF& point)
{
    _transform->set_scale(point.x(), point.y());
}

void ImageTexture::setScale(float s)
{
    _transform->set_scale(s);
}

void ImageTexture::setScale(const QMatrix4x4& point)
{
    _transform->set_scale(point);
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
    _transform->add_trans(x, y);
}

void ImageTexture::addScale(float x, float y)
{
    _transform->add_scale(x, y);
}

void ImageTexture::addScale(float s)
{
    addScale(s, s);
}

void ImageTexture::addRot(float r)
{
    _transform->add_rot(r);
}
QMatrix4x4 ImageTexture::getMatrix() const {
    return _transform->getMatrix();
}

QMatrix4x4 ImageTexture::getMatrixInvert() const
{
    return _transform->getMatrixInvert();
}

void ImageTexture::resetTransform() {
    _transform->reset();
}

void ImageTexture::bind()
{
    _texture->bind();
}

void ImageTexture::operator=(const ImageTransform& transform) {
    *_transform = transform;
}

const ImageTransform& ImageTexture::transform() const {
    return *_transform;
}