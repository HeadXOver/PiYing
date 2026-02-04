#pragma once

#include <memory>
#include <qmatrix4x4>

class ImageTransform
{
public:
    ImageTransform();
    ~ImageTransform();

    void operator=(const ImageTransform& other);

    QMatrix4x4 getMatrix() const;
    QMatrix4x4 getMatrixInvert() const;

    float get_trans_x() const;
    float get_trans_y() const;
    float get_rot_degree() const;
    float get_scale_x() const;
    float get_scale_y() const;

    void reset();
    void set_trans(float x, float y);
    void set_trans(const QMatrix4x4& m);
    void set_rot(float degree);
    void set_rot(const QMatrix4x4& m);
    void set_scale(float x, float y);
    void set_scale(float s);
    void set_scale(const QMatrix4x4& m);

    void add_trans(float x, float y);
    void add_rot(float degree);
    void add_scale(float x, float y);

    const QMatrix4x4& trans() const;
    const QMatrix4x4& rot() const;
    const QMatrix4x4& scale() const;

private:
    QMatrix4x4 trans_;
    QMatrix4x4 rot_;
    QMatrix4x4 scale_;
};

