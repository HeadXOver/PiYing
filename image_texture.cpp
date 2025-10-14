#include "image_texture.h"

#include <qopengltexture>

ImageTexture::ImageTexture(QImage& image)
{
    tex = new QOpenGLTexture(image.flipped());
    tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    tex->setMagnificationFilter(QOpenGLTexture::Linear);
    tex->setWrapMode(QOpenGLTexture::Repeat);
}

void ImageTexture::setTrans(float x, float y)
{
    transform.trans.setToIdentity();
    transform.trans.translate(x, y);
}

void ImageTexture::setTrans(QPointF point)
{
    transform.trans.setToIdentity();
    transform.trans.translate(point.x(), point.y());
}

void ImageTexture::setRot(float r)
{
    transform.rot.setToIdentity();
    transform.rot.rotate(r, 0.f, 0.f, 1.0f);
}

void ImageTexture::setScale(float x, float y)
{
    transform.scale.setToIdentity();
    transform.scale.scale(x, y);
}

void ImageTexture::setScale(QPointF point)
{
    transform.scale.setToIdentity();
    transform.scale.scale(point.x(), point.y());
}

void ImageTexture::setScale(float s)
{
    transform.scale.setToIdentity();
    transform.scale.scale(s);
}

void ImageTexture::addTrans(QPointF point)
{
    addTrans(point.x(), point.y());
}

void ImageTexture::addScale(QPointF point)
{
    transform.scale.scale(point.x(), point.y());
}

void ImageTexture::addTrans(float x, float y)
{
    transform.trans.translate(x, y);
}

void ImageTexture::addScale(float x, float y)
{
    transform.scale.scale(x, y);
}

void ImageTexture::addScale(float s)
{
    transform.scale.scale(s);
}

void ImageTexture::addRot(float r)
{
    transform.rot.rotate(r, 0.0f, 0.0f, 1.0f);
}
QMatrix4x4 ImageTexture::getMatrix() const {
    return transform.getMatrix(); 
}

QMatrix4x4 ImageTexture::getMatrixInvert() const
{
    return transform.getMatrixInvert(); 
}

void ImageTexture::reset() {
    transform.reset();
}