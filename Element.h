#pragma once

#include "CusEnum.h"

struct ImageTransform {
    QMatrix4x4 getMatrix() const         { return trans * rot * scale; }
    QMatrix4x4 getMatrixInvert() const   { return scale.inverted() * rot.inverted() * trans.inverted(); }

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
    }

    void setTrans(float x, float y) {
        transform.trans.setToIdentity();
        transform.trans.translate(x, y);
    }

    void setTrans(QPointF point) {
        transform.trans.setToIdentity();
        transform.trans.translate(point.x(), point.y());
    }

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

    void setScale(float s) {
        transform.scale.setToIdentity();
        transform.scale.scale(s);
    }

    void addTrans(QPointF point)        { addTrans(point.x(), point.y()); }
    void addScale(QPointF point)        { transform.scale.scale(point.x(), point.y()); }
    void addTrans(float x, float y)     { transform.trans.translate(x, y); }
    void addScale(float x, float y)     { transform.scale.scale(x, y); }
    void addScale(float s)              { transform.scale.scale(s); }
    void addRot(float r)                { transform.rot.rotate(r, 0.0f, 0.0f, 1.0f); }

    QMatrix4x4 getMatrix() const        { return transform.getMatrix(); }
    QMatrix4x4 getMatrixInvert() const  { return transform.getMatrixInvert(); }

    QOpenGLTexture* tex;

    ImageTransform transform;
};

const float RECTANGLE_VERT[8] = {
     1.0,  1.0,
     1.0, -1.0,
    -1.0, -1.0,
    -1.0,  1.0
};

const unsigned int RECTANGLE_INDECES[6] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

struct First2Vert {
    QPointF first;
    QPointF second;
};

struct First2Index {
    unsigned int first = -1;
    unsigned int second = -1;
};

struct ToolButton {
    ToolButton(QString selectedFileName, QString unselectedFileName, QString actionName, ToolState state) {
        selected = QIcon(selectedFileName);
        unselected = QIcon(unselectedFileName);
        action = new QAction(unselected, actionName);
        toolState = state;
    }

    void select()   { action->setIcon(selected); }
    void unSelect() { action->setIcon(unselected); }

    QAction* action;
    QIcon selected;
    QIcon unselected;
    ToolState toolState;
};