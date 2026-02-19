#include "character_texture.h"
#include "image_transform.h"

#include <qmatrix4x4>
#include <qopengltexture>
#include <qimage>
#include <qpointf>

namespace {
    constexpr float DEFAULT_RATIO = 16.f / 9.f;
}

CharacterTexture::CharacterTexture(const QImage& image):
    _prescale(DEFAULT_RATIO * float(image.height()) / (float)image.width()),
    _texture(image.flipped())
{
    _texture.setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _texture.setMagnificationFilter(QOpenGLTexture::Linear);
    _texture.setWrapMode(QOpenGLTexture::ClampToEdge);
}

QMatrix4x4 CharacterTexture::getMatrix() const noexcept
{
    QMatrix4x4 matrix;
    matrix.scale(1.f, _prescale);
    return matrix;
}

QOpenGLTexture& CharacterTexture::texture() noexcept
{
    return _texture;
}

void CharacterTexture::bind()
{
    _texture.bind();
}