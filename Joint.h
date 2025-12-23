#pragma once

class QMatrix4x4;

class Joint final
{
public:
    Joint() = default;
    QMatrix4x4 get_local_transform() const;

private:
    float trans_x{ 0 };
    float trans_y{ 0 };

    float rot{ 0 };

    float scale_x{ 1.f };
    float scale_y{ 1.f };
};