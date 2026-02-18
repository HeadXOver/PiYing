#pragma once

class QMatrix4x4;

class Joint final
{
public:
    Joint() = default;
    Joint& operator=(const Joint& other) = default;

    QMatrix4x4 get_local_transform() const noexcept;

private:
    float 
        center_x{ 0 },
        center_y{ 0 },
        trans_x{ 0 },
        trans_y{ 0 },
        rot{ 0 },
        scale_x{ 1.f },
        scale_y{ 1.f };
};