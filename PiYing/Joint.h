#pragma once

#include <qmatrix4x4>

class QPointF;

class Joint final
{
public:
    Joint() = default;
    Joint& operator=(const Joint& other) = default;

    void update_matrix() noexcept;

    void set_center(float x, float y) noexcept;
    void set_center(const QPointF& center) noexcept;
    void set_rotation(float r) noexcept { rot = r; update_matrix(); }
    void set_scale_x(float s) noexcept { scale_x = s; update_matrix(); }
    void set_scale_y(float s) noexcept { scale_y = s; update_matrix(); }
    void set_trans_x(float t) noexcept { trans_x = t; update_matrix(); }
    void set_trans_y(float t) noexcept { trans_y = t; update_matrix(); }
    void set_center_x(float x) noexcept { center_x = x; update_matrix(); }
    void set_center_y(float y) noexcept { center_y = y; update_matrix(); }

    QPointF get_center() const noexcept;

    float get_rotation() const noexcept { return rot; }
    float get_scale_x() const noexcept { return scale_x; }
    float get_scale_y() const noexcept { return scale_y; }
    float get_trans_x() const noexcept { return trans_x; }
    float get_trans_y() const noexcept { return trans_y; }
    float get_center_x() const noexcept { return center_x; }
    float get_center_y() const noexcept { return center_y; }

    const QMatrix4x4& get_matrix() const noexcept { return localTransform; }


private:
    float 
        center_x{ 0 },
        center_y{ 0 },
        trans_x{ 0 },
        trans_y{ 0 },
        rot{ 0 },
        scale_x{ 1.f },
        scale_y{ 1.f };

    QMatrix4x4 localTransform;   // 相对于父节点
};