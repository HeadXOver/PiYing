#pragma once

#include <memory>
#include <qopengltexture>

class ImageTransform;
class QImage;
class QPointF;
class QMatrix4x4;

namespace piying {

    class Texture
    {
    public:
        Texture(const QImage& image);
        ~Texture() = default;

        void bind() { _texture.bind(); }

        float get_prescale() const noexcept { return _prescale; }

        QOpenGLTexture& texture() noexcept { return _texture; }

        QMatrix4x4 getMatrix() const noexcept;

    protected:
        float _prescale;
        QOpenGLTexture _texture;
    };

    class ImageTexture final : private Texture
    {
    public:
        ImageTexture(const QImage& image, float currentRatio);
        ~ImageTexture();

        void set_transform(const ImageTransform& transform) noexcept;
        void set_transform(const ImageTransform* transform) noexcept;

        void setTrans(float x, float y) noexcept;
        void set_rot_radian(float r) noexcept;
        void setScale(float x, float y) noexcept;
        void setScale(float s) noexcept;

        void addTrans(const QPointF& point) noexcept;
        void addTrans(float x, float y) noexcept;
        void addScale(float x, float y) noexcept;
        void addScale(float s) noexcept;
        void add_rot_radian(float r) noexcept;

        void resetTransform() noexcept;
        void bind();

        void set_transform_by_new_ratio(float newRatio);

        const ImageTransform& transform() const noexcept;

        QMatrix4x4 getMatrix() const noexcept;
        QMatrix4x4 getMatrixInvert() const noexcept;

    private:
        std::unique_ptr<ImageTransform> _transform;
    };

}
