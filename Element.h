#pragma once

#include <QOpenGLTexture>
#include <QMatrix4x4>

struct ImageTransform {
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
        trans.setToIdentity();
        rot.setToIdentity();
        scale.setToIdentity();
        //rot.rotate(30.f, 0.f, 0.f, 1.0f);
        //scale.scale(0.5f, 0.5f, 1.0f);
    }

    ~ImageTexture() {

    }

    void setTrans(float x, float y) {
        trans.setToIdentity();
        trans.translate(x, y);
    }

    void setTrans(QPointF point) {
        trans.setToIdentity();
        trans.translate(point.x(), point.y());
    }

    inline void addTrans(QPointF point) { trans.translate(point.x(), point.y()); }

    void setRot(float r) {
        rot.setToIdentity();
        rot.rotate(r, 0.f, 0.f, 1.0f);
    }

    void setScale(float x, float y) {
        scale.setToIdentity();
        scale.scale(x, y);
    }

    void setScale(QPointF point) {
        scale.setToIdentity();
        scale.scale(point.x(), point.y());
    }

    inline void addScale(QPointF point) { scale.scale(point.x(), point.y()); }
    inline void addScale(float x, float y) { scale.scale(x, y); }

    void setScale(float s) {
		scale.setToIdentity();
		scale.scale(s);
	}

    QOpenGLTexture* tex;
    QMatrix4x4 trans;
    QMatrix4x4 rot;
    QMatrix4x4 scale;
    
	bool selected = false;
};

static const float RECTANGLE_VERT[] = {
    // positions          // texture coords
     1.0,  1.0, 0.0f,   1.0, 1.0,   // top right
     1.0, -1.0, 0.0f,   1.0, 0.0f,  // bottom right
    -1.0, -1.0, 0.0f,   0.0f, 0.0f, // bottom left
    -1.0,  1.0, 0.0f,   0.0f, 1.0   // top left 
};

static const unsigned int RECTANGLE_INDECES[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};