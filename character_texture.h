#pragma once

#include <memory>

class QOpenGLTexture;
class ImageTransform;
class QImage;
class QPointF;
class QMatrix4x4;

class CharacterTexture final
{
public:
    CharacterTexture(const QImage& image);
    ~CharacterTexture();

    void bind();

    float get_prescale() const { return _prescale; }

    QMatrix4x4 getMatrix() const;

    QOpenGLTexture* texture() const;

private:
    const std::unique_ptr<QOpenGLTexture> _texture;

    const float _prescale;
};

