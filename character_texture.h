#pragma once

#include <memory>
#include <qopengltexture>

class QImage;
class QMatrix4x4;

class CharacterTexture final
{
public:
    CharacterTexture(const QImage& image);
    ~CharacterTexture() = default;

    void bind() { _texture.bind(); }

    float get_prescale() const noexcept { return _prescale; }

    QOpenGLTexture& texture() noexcept { return _texture; }

    QMatrix4x4 getMatrix() const noexcept;

private:
    QOpenGLTexture _texture;

    const float _prescale;
};

