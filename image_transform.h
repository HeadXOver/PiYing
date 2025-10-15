#pragma once

class QMatrix4x4;

class ImageTransform
{
public:
    ImageTransform();
    ~ImageTransform();

    QMatrix4x4 getMatrix() const;
    QMatrix4x4 getMatrixInvert() const;

    float get_trans_x() const;
    float get_trans_y() const;
    float get_rot_degree() const;
    float get_scale_x() const;
    float get_scale_y() const;

    void reset();
    void set_trans(const QMatrix4x4& m);
    void set_rot(const QMatrix4x4& m);
    void set_scale(const QMatrix4x4& m);
    void copy_from(const ImageTransform& other);
    void copy_from(const ImageTransform* other);

    QMatrix4x4 get_trans() const;
    QMatrix4x4 get_rot() const;
    QMatrix4x4 get_scale() const;

    QMatrix4x4* trans() const;
    QMatrix4x4* rot() const;
    QMatrix4x4* scale() const;

private:
    QMatrix4x4* trans_;
    QMatrix4x4* rot_;
    QMatrix4x4* scale_;
};

