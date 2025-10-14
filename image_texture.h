#pragma once

#include <qpointf>
#include <qmatrix4x4>

#include "image_transform.h"

class QOpenGLTexture;

class ImageTexture 
{
public:
    ImageTexture(QImage& image);

    void setTrans(float x, float y);
    void setTrans(QPointF point);
    void setRot(float r);
    void setScale(float x, float y);
    void setScale(QPointF point);
    void setScale(float s);

    void addTrans(QPointF point);
    void addScale(QPointF point);
    void addTrans(float x, float y);
    void addScale(float x, float y);
    void addScale(float s);
    void addRot(float r);

    void reset();

    QMatrix4x4 getMatrix() const;
    QMatrix4x4 getMatrixInvert() const;

    QOpenGLTexture* tex;

    ImageTransform transform;
};

