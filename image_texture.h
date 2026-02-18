#pragma once

#include <memory>

class QOpenGLTexture;
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

    void setTrans(float x, float y);
    void setTrans(const QPointF& point);
    void setRot(float r);
    void setScale(float x, float y);
    void setScale(const QPointF& point);
    void setScale(float s);

    void addTrans(const QPointF& point);
    void addScale(const QPointF& point);
    void addTrans(float x, float y);
    void addScale(float x, float y);
    void addScale(float s);
    void addRot(float r);

    void resetTransform();
    void bind();

    void set_transform_by_new_ratio(float newRatio);

    const ImageTransform& transform() const noexcept;

    QMatrix4x4 getMatrix() const noexcept;
    QMatrix4x4 getMatrixInvert() const noexcept;

    QOpenGLTexture* texture() const noexcept;

private:
    std::unique_ptr<QOpenGLTexture> _texture;

    std::unique_ptr<ImageTransform> _transform;

    float _prescale;
};

