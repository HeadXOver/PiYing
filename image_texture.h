#pragma once

#include <memory>
#include <qopengltexture>

class ImageTransform;
class QImage;
class QPointF;
class QMatrix4x4;

class ImageTexture final
{
public:
    ImageTexture(const QImage& image, float currentRatio);
    ~ImageTexture();

    void set_transform(const ImageTransform& transform) noexcept;
    void set_transform(const ImageTransform* transform) noexcept;

    void setTrans(float x, float y) noexcept;
    void setTrans(const QPointF& point) noexcept;
    void setRot(float r) noexcept;
    void setScale(float x, float y) noexcept;
    void setScale(const QPointF& point) noexcept;
    void setScale(float s) noexcept;

    void addTrans(const QPointF& point) noexcept;
    void addScale(const QPointF& point) noexcept;
    void addTrans(float x, float y) noexcept;
    void addScale(float x, float y) noexcept;
    void addScale(float s) noexcept;
    void addRot(float r) noexcept;

    void resetTransform() noexcept;
    void bind();

    void set_transform_by_new_ratio(float newRatio);

    const ImageTransform& transform() const noexcept;

    QMatrix4x4 getMatrix() const noexcept;
    QMatrix4x4 getMatrixInvert() const noexcept;

private:
    QOpenGLTexture _texture;

    std::unique_ptr<ImageTransform> _transform;

    float _prescale;
};

