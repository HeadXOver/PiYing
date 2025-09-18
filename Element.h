#pragma once

#include <QOpenGLTexture>
#include <QMatrix4x4>

struct ImageTexture {

    ImageTexture(QImage& image) {
        tex = new QOpenGLTexture(image.flipped());
        tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        tex->setMagnificationFilter(QOpenGLTexture::Linear);
        tex->setWrapMode(QOpenGLTexture::Repeat);
        model.setToIdentity();
		model.translate(-image.width() / 2.0, -image.height() / 2.0, 0); // 把图像中心移到原点
    }

    ~ImageTexture() {
    }

    QOpenGLTexture* tex;
    QMatrix4x4      model;   // 这张图的模型矩阵
};

static const float RECTANGLE_VERT[] = {
    // positions          // texture coords
     0.5f,  0.5f, 0.0f,   1.0, 1.0, // top right
     0.5f, -0.5f, 0.0f,   1.0, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0  // top left 
};

static const unsigned int RECTANGLE_INDECES[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};