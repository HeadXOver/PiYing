#include "piying_texture.h"
#include "image_transform.h"

#include <qmatrix4x4>
#include <qopengltexture>
#include <qimage>
#include <qpointf>

namespace {
    constexpr float DEFAULT_RATIO = 16.f / 9.f;
}

piying::ImageTexture::ImageTexture(const QImage& image, float currentRatio) :
    Texture(image.flipped()),
    _transform(std::make_unique<ImageTransform>())
{
    _prescale = image.height() * currentRatio / static_cast<float>(image.width());
    _transform->set_scale(1.f, _prescale);

    _texture.setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _texture.setMagnificationFilter(QOpenGLTexture::Linear);
}

piying::ImageTexture::~ImageTexture() {}

void piying::ImageTexture::setTrans(float x, float y) noexcept
{
    _transform->set_trans(x, y);
}

void piying::ImageTexture::setTrans(const QPointF& point) noexcept
{
    _transform->set_trans(point.x(), point.y());
}

void piying::ImageTexture::set_rot_radian(float r) noexcept
{
    _transform->set_rot_radian(r);
}

void piying::ImageTexture::setScale(float x, float y) noexcept
{
    _transform->set_scale(x, y * _prescale);
}

void piying::ImageTexture::setScale(const QPointF& point) noexcept
{
    _transform->set_scale(point.x(), point.y());
}

void piying::ImageTexture::setScale(float s) noexcept
{
    _transform->set_scale(s, s * _prescale);
}

void piying::ImageTexture::addTrans(const QPointF& point) noexcept
{
    addTrans(point.x(), point.y());
}

void piying::ImageTexture::addScale(const QPointF& point) noexcept
{
    addScale(point.x(), point.y());
}

void piying::ImageTexture::addTrans(float x, float y) noexcept
{
    _transform->add_trans(x, y);
}

void piying::ImageTexture::addScale(float x, float y) noexcept
{
    _transform->add_scale(x, y);
}

void piying::ImageTexture::addScale(float s) noexcept
{
    addScale(s, s);
}

void piying::ImageTexture::add_rot_radian(float r) noexcept
{
    _transform->add_rot_radian(r);
}

QMatrix4x4 piying::ImageTexture::getMatrix() const noexcept
{
    return _transform->getMatrix();
}

QMatrix4x4 piying::ImageTexture::getMatrixInvert() const noexcept
{
    return _transform->getMatrixInvert();
}

void piying::ImageTexture::resetTransform() noexcept
{ 
    _transform->reset();
    _transform->set_scale(1.f, _prescale);
}

void piying::ImageTexture::bind()
{
    _texture.bind();
}

void piying::ImageTexture::set_transform_by_new_ratio(float newRatio)
{
    const float oldScale = _prescale;
    _prescale = _texture.height() * newRatio / static_cast<float>(_texture.width());

    const float oldNewRatio = _prescale / oldScale;

    _transform->add_scale(1.0f, oldNewRatio);

    const float oldX = _transform->get_trans_x();
    const float oldY = _transform->get_trans_y();
    _transform->set_trans(oldX * oldNewRatio, oldY * oldNewRatio);
}

void piying::ImageTexture::set_transform(const ImageTransform& transform) noexcept
{
    *_transform = transform;
}

void piying::ImageTexture::set_transform(const ImageTransform* transform) noexcept
{
    *_transform = *transform;
}

const ImageTransform& piying::ImageTexture::transform() const noexcept
{
    return *_transform;
}