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
    const float scaleMatrix[16] = {
        1.f, 0.f, 0.f, 0.f,
        0.f, _prescale, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f
    };
    return QMatrix4x4(scaleMatrix);
}

void CharacterTexture::bind()
{
    _texture.bind();
}