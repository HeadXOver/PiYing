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
    ImageTexture(const QImage& image);
    ~ImageTexture();

    void set_transform(const ImageTransform& transform);
    void set_transform(const ImageTransform* transform);

    void setTrans(float x, float y);
    void setTrans(const QPointF& point);
    void setTrans(const QMatrix4x4& point);
    void setRot(float r);
    void setRot(const QMatrix4x4& rot);
    void setScale(float x, float y);
    void setScale(const QPointF& point);
    void setScale(float s);
    void setScale(const QMatrix4x4& point);

    void addTrans(const QPointF& point);
    void addScale(const QPointF& point);
    void addTrans(float x, float y);
    void addScale(float x, float y);
    void addScale(float s);
    void addRot(float r);

    void resetTransform();
    void bind();

    void set_transform_by_new_ratio(float newRatio);

    float get_prescale() const { return _prescale; }

    const ImageTransform& transform() const;

    QMatrix4x4 getMatrix() const;
    QMatrix4x4 getMatrixInvert() const;

    QOpenGLTexture* texture() const;

private:
    std::unique_ptr<QOpenGLTexture> _texture;

    std::unique_ptr<ImageTransform> _transform;

    float _prescale{ 1.0f };
    float _image_ratio{ 1.0f };
};

