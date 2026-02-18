#pragma once

#include <memory>
#include <qmatrix4x4>

class ImageTransform
{
public:
    ImageTransform();
    ~ImageTransform();

    void operator=(const ImageTransform& other) noexcept;

    const QMatrix4x4& getMatrix() const noexcept;
    const QMatrix4x4& getMatrixInvert() const noexcept;

    float get_trans_x() const noexcept;
    float get_trans_y() const noexcept;
    float get_rot_degree() const noexcept;
    float get_scale_x() const noexcept;
    float get_scale_y() const noexcept;

    void reset() noexcept;
    void set_trans(float x, float y) noexcept;
    void set_rot(float degree) noexcept;
    void set_scale(float x, float y) noexcept;
    void set_scale(float s) noexcept;

    void add_trans(float x, float y) noexcept;
    void add_rot(float degree) noexcept;
    void add_scale(float x, float y) noexcept;

    QMatrix4x4 trans() const noexcept;
    QMatrix4x4 rot() const noexcept;
    QMatrix4x4 scale() const noexcept;

private:
    void update_matrix() noexcept;

private:
    QMatrix4x4 _transform;
    QMatrix4x4 _transform_invert;

    float _trans_x{ 0 };
    float _trans_y{ 0 };
    float _rot_degree{ 0 };
    float _scale_x{ 1 };
    float _scale_y{ 1 };
};

