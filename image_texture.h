#pragma once

class QOpenGLTexture;
class ImageTransform;
class QImage;
class QPointF;
class QMatrix4x4;

class ImageTexture final
{
public:
    ImageTexture(QImage& image);
    ~ImageTexture();

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

    void copyTransformFrom(ImageTransform* transform);

    ImageTransform* transform() const;

    QMatrix4x4 getMatrix() const;
    QMatrix4x4 getMatrixInvert() const;

    QOpenGLTexture * texture() const;

private:

    QOpenGLTexture* texture_;

    ImageTransform* transform_ = nullptr;
};

