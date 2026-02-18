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
    _prescale = image.height() * currentRatio / static_cast<float>(image.width());

    _transform = std::make_unique<ImageTransform>();

    _transform->set_scale(1.f, _prescale);

    _texture = std::make_unique<QOpenGLTexture>(image.flipped());
    _texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _texture->setMagnificationFilter(QOpenGLTexture::Linear);
}

ImageTexture::~ImageTexture()
{
}

void ImageTexture::setTrans(float x, float y) noexcept
{
    _transform->set_trans(x, y);
}

void ImageTexture::setTrans(const QPointF& point) noexcept
{
    _transform->set_trans(point.x(), point.y());
}

void ImageTexture::setRot(float r) noexcept
{
    _transform->set_rot(r);
}

void ImageTexture::setScale(float x, float y) noexcept
{
    _transform->set_scale(x, y * _prescale);
}

void ImageTexture::setScale(const QPointF& point) noexcept
{
    _transform->set_scale(point.x(), point.y());
}

void ImageTexture::setScale(float s) noexcept
{
    _transform->set_scale(s, s * _prescale);
}

void ImageTexture::addTrans(const QPointF& point) noexcept
{
    addTrans(point.x(), point.y());
}

void ImageTexture::addScale(const QPointF& point) noexcept
{
    addScale(point.x(), point.y());
}

void ImageTexture::addTrans(float x, float y) noexcept
{
    _transform->add_trans(x, y);
}

void ImageTexture::addScale(float x, float y) noexcept
{
    _transform->add_scale(x, y);
}

void ImageTexture::addScale(float s) noexcept
{
    addScale(s, s);
}

void ImageTexture::addRot(float r) noexcept
{
    _transform->add_rot(r);
}

QMatrix4x4 ImageTexture::getMatrix() const noexcept
{
    return _transform->getMatrix();
}

QMatrix4x4 ImageTexture::getMatrixInvert() const noexcept
{
    return _transform->getMatrixInvert();
}

QOpenGLTexture* ImageTexture::texture() const noexcept
{
    return _texture.get();
}

void ImageTexture::resetTransform() noexcept
{ 
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
    _prescale = _texture->height() * newRatio / static_cast<float>(_texture->width());

    const float oldNewRatio = _prescale / oldScale;

    _transform->add_scale(1.0f, oldNewRatio);

    const float oldX = _transform->get_trans_x();
    const float oldY = _transform->get_trans_y();
    _transform->set_trans(oldX * oldNewRatio, oldY * oldNewRatio);
}

void ImageTexture::set_transform(const ImageTransform& transform) noexcept
{
    *_transform = transform;
}

void ImageTexture::set_transform(const ImageTransform* transform) noexcept
{
    *_transform = *transform;
}

const ImageTransform& ImageTexture::transform() const noexcept
{
    return *_transform;
}