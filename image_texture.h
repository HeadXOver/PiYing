#pragma once

#include <qpointf>
#include <qmatrix4x4>

class QOpenGLTexture;
class ImageTransform;

class ImageTexture 
{
public:
    ImageTexture(QImage& image);
    ~ImageTexture();

    void setTrans(float x, float y);
    void setTrans(QPointF point);
    void setTrans(QMatrix4x4 point);
    void setRot(float r);
    void setRot(QMatrix4x4 r);
    void setScale(float x, float y);
    void setScale(QPointF point);
    void setScale(float s);
    void setScale(QMatrix4x4 scale);

    void addTrans(QPointF point);
    void addScale(QPointF point);
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

