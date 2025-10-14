#pragma once

#include <qmatrix4x4>

class ImageTransform
{
public:
    QMatrix4x4 getMatrix() const { return trans * rot * scale; }
    QMatrix4x4 getMatrixInvert() const { return scale.inverted() * rot.inverted() * trans.inverted(); }

    void reset();

    QMatrix4x4 trans;
    QMatrix4x4 rot;
    QMatrix4x4 scale;
};

