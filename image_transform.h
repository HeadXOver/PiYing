#pragma once

#include <memory>
#include <qmatrix4x4>

class ImageTransform
{
public:
    ImageTransform& operator=(const ImageTransform& other) = default;

    const QMatrix4x4& getMatrix() const noexcept { return _transform; }
    const QMatrix4x4& getMatrixInvert() const noexcept { return _transform_invert; }

    float get_trans_x() const noexcept { return _trans_x; }
    float get_trans_y() const noexcept { return _trans_y; }
    float get_rot_degree() const noexcept { return 180 * _rot_radian / 3.1415927f; }
    float get_rot_radian() const noexcept { return _rot_radian; }
    float get_scale_x() const noexcept { return _scale_x; }
    float get_scale_y() const noexcept { return _scale_y; }

    void reset() noexcept;
    void set_trans(float x, float y) noexcept;
    void set_rot_radian(float degree) noexcept;
    void set_scale(float x, float y) noexcept;
    void set_scale(float s) noexcept;

    void add_trans(float x, float y) noexcept;
    void add_rot_radian(float degree) noexcept;
    void add_scale(float x, float y) noexcept;

    QMatrix4x4 trans() const noexcept;
    QMatrix4x4 rot() const noexcept;
    QMatrix4x4 scale() const noexcept;

private:
    void update_2_matrix() noexcept;
    void update_matrix_invert() noexcept;

private:
    QMatrix4x4 _transform;
    QMatrix4x4 _transform_invert;

    float _trans_x{ 0 };
    float _trans_y{ 0 };
    float _rot_radian{ 0 };
    float _scale_x{ 1 };
    float _scale_y{ 1 };
};

