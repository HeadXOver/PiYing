#pragma once

#include <memory>
#include <qmatrix4x4>

class ImageTransform
{
public:
    ImageTransform();
    ~ImageTransform();

    void operator=(const ImageTransform& other);

    const QMatrix4x4& getMatrix() const;
    const QMatrix4x4& getMatrixInvert() const;

    float get_trans_x() const;
    float get_trans_y() const;
    float get_rot_degree() const;
    float get_scale_x() const;
    float get_scale_y() const;

    void reset();
    void set_trans(float x, float y);
    void set_rot(float degree);
    void set_scale(float x, float y);
    void set_scale(float s);

    void add_trans(float x, float y);
    void add_rot(float degree);
    void add_scale(float x, float y);

    QMatrix4x4 trans() const;
    QMatrix4x4 rot() const;
    QMatrix4x4 scale() const;

private:
    void update_matrix();

private:
    QMatrix4x4 _transform;
    QMatrix4x4 _transform_invert;

    float _trans_x{ 0 };
    float _trans_y{ 0 };
    float _rot_degree{ 0 };
    float _scale_x{ 1 };
    float _scale_y{ 1 };
};

