#pragma once

#include <memory>

class QOpenGLTexture;
class QImage;
class QMatrix4x4;

class CharacterTexture final
{
public:
    CharacterTexture(const QImage& image);
    ~CharacterTexture();

    void bind();

    float get_prescale() const noexcept { return _prescale; }

    QMatrix4x4 getMatrix() const noexcept;

    QOpenGLTexture* texture() const noexcept;

private:
    const std::unique_ptr<QOpenGLTexture> _texture;

    const float _prescale;
};

