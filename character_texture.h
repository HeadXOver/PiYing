#pragma once

#include <memory>
#include <qopengltexture>

class QOpenGLTexture;
class QImage;
class QMatrix4x4;

class CharacterTexture final
{
public:
    CharacterTexture(const QImage& image);
    ~CharacterTexture() = default;

    void bind();

    float get_prescale() const noexcept { return _prescale; }

    QMatrix4x4 getMatrix() const noexcept;

    QOpenGLTexture& texture() noexcept { return _texture; }

private:
    QOpenGLTexture _texture;

    const float _prescale;
};

