#include "image_texture.h"
#include "image_transform.h"

#include <qmatrix4x4>
#include <qopengltexture>
#include <qimage>
#include <qpointf>

namespace {
    constexpr float DEFAULT_RATIO = 16.f / 9.f;
}

ImageTexture::ImageTexture(const QImage& image, float currentRatio)
{
    _image_ratio = image.width() / float(image.height());
    _prescale = currentRatio / _image_ratio;

    _transform = std::make_unique<ImageTransform>();

    _transform->set_scale(1.f, _prescale);

    _texture = std::make_unique<QOpenGLTexture>(image.flipped());
    _texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _texture->setMagnificationFilter(QOpenGLTexture::Linear);
    _texture->setWrapMode(QOpenGLTexture::ClampToEdge);
}

ImageTexture::ImageTexture(const QImage& image)
{
    _image_ratio = image.width() / float(image.height());
    _prescale = DEFAULT_RATIO / _image_ratio;

    _transform = std::make_unique<ImageTransform>();

    _transform->set_scale(1.f, _prescale);

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
    _transform->set_scale(x, y * _prescale);
}

void ImageTexture::setScale(const QPointF& point)
{
    _transform->set_scale(point.x(), point.y());
}

void ImageTexture::setScale(float s)
{
    _transform->set_scale(s, s * _prescale);
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

QMatrix4x4 ImageTexture::getMatrix() const 
{
    return _transform->getMatrix();
}

QMatrix4x4 ImageTexture::getMatrixInvert() const
{
    return _transform->getMatrixInvert();
}

QOpenGLTexture* ImageTexture::texture() const
{
    return _texture.get();
}

void ImageTexture::resetTransform() {
    _transform->reset();
    _transform->set_scale(1.f, _prescale);
}

void ImageTexture::bind()
{
    _texture->bind();
}

void ImageTexture::set_transform_by_new_ratio(float newRatio)
{
    const float oldScale = _prescale;
    _prescale = newRatio / _image_ratio;

    const float oldNewRatio = _prescale / oldScale;

    _transform->add_scale(1.0f, oldNewRatio);

    const float oldX = _transform->get_trans_x();
    const float oldY = _transform->get_trans_y();
    _transform->set_trans(oldX * oldNewRatio, oldY * oldNewRatio);
}

void ImageTexture::set_transform(const ImageTransform& transform) {
    *_transform = transform;
}

void ImageTexture::set_transform(const ImageTransform* transform)
{
    *_transform = *transform;
}

const ImageTransform& ImageTexture::transform() const {
    return *_transform;
}