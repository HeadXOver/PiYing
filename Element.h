#pragma once

#include <QOpenGLTexture>
#include <QMatrix4x4>

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