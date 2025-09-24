#pragma once

#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <vector>
#include <QLineF>

struct ImageTransform {
    inline QMatrix4x4 getMatrix() const{
        return trans * rot * scale;
    }

    inline QMatrix4x4 getMatrixInvert() const {
        return scale.inverted() * rot.inverted() * trans.inverted();
    }

    void reset() {
        trans.setToIdentity();
        rot.setToIdentity();
        scale.setToIdentity();
    }

    QMatrix4x4 trans;
    QMatrix4x4 rot;
    QMatrix4x4 scale;
};

struct ImageTexture {

    ImageTexture(QImage& image) {
        tex = new QOpenGLTexture(image.flipped());
        tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        tex->setMagnificationFilter(QOpenGLTexture::Linear);
        tex->setWrapMode(QOpenGLTexture::Repeat);
        //rot.rotate(30.f, 0.f, 0.f, 1.0f);
        //scale.scale(0.5f, 0.5f, 1.0f);
    }

    ~ImageTexture() {

    }

    void setTrans(float x, float y) {
        transform.trans.setToIdentity();
        transform.trans.translate(x, y);
    }

    void setTrans(QPointF point) {
        transform.trans.setToIdentity();
        transform.trans.translate(point.x(), point.y());
    }

    inline void addTrans(QPointF point) { addTrans(point.x(), point.y()); }
    inline void addTrans(float x, float y) { transform.trans.translate(x, y); }

    void setRot(float r) {
        transform.rot.setToIdentity();
        transform.rot.rotate(r, 0.f, 0.f, 1.0f);
    }

    void setScale(float x, float y) {
        transform.scale.setToIdentity();
        transform.scale.scale(x, y);
    }

    void setScale(QPointF point) {
        transform.scale.setToIdentity();
        transform.scale.scale(point.x(), point.y());
    }

    inline void addScale(QPointF point) { transform.scale.scale(point.x(), point.y()); }
    inline void addScale(float x, float y) { transform.scale.scale(x, y); }
    inline void addScale(float s) { transform.scale.scale(s); }

    void setScale(float s) {
        transform.scale.setToIdentity();
        transform.scale.scale(s);
	}

    inline void addRot(float r) { transform.rot.rotate(r, 0.0f, 0.0f, 1.0f); }

    inline QMatrix4x4 getMatrix() const{
        return transform.getMatrix();
    }

    inline QMatrix4x4 getMatrixInvert() const {
        return transform.getMatrixInvert();
    }

    QOpenGLTexture* tex;

    ImageTransform transform;
};

static const float RECTANGLE_VERT[] = {
    // positions
     1.0,  1.0,
     1.0, -1.0,
    -1.0, -1.0,
    -1.0,  1.0
};

static const unsigned int RECTANGLE_INDECES[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

static QMatrix4x4 getTrans(const QMatrix4x4& M) {
    QMatrix4x4 trans;
    trans.translate(M(0, 3), M(1, 3));
    return trans;
}

static QMatrix4x4 getRot(const QMatrix4x4& M) {
    QMatrix4x4 trans;
    trans.rotate(qAtan2(M(1, 0), M(0, 0)) * 180.f / 3.141593, 0.0f, 0.0f, 1.0f);
    return trans;
}

static QMatrix4x4 getScale(const QMatrix4x4& M) {
    QMatrix4x4 trans;
    trans.scale(qSqrt(M(0, 0) * M(0, 0) + M(1, 0) * M(1, 0)));
    return trans;
}

enum class First2VertState {
    None,
    HalfPoint,
    HalfSelect,
    Full2Point,
    Full2Select,
    FullSelectPoint,
    FullPointSelect,
};

struct First2Vert {
    QPointF first;
    QPointF second;
};

struct First2Index {
    unsigned int first = -1;
    unsigned int second = -1;
};