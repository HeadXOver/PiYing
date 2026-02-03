#include "piYingGL.h"

#include "image_transform.h"

QPointF PiYingGL::getRaletiveToRect(const QPointF& point, const QMatrix4x4& transform) const
{
	return (proj * transform * getViewMatrixInvert() * insProj).map(point);
}

QPointF PiYingGL::mapToGL(const QPointF& point) const
{ 
	return QPointF((point.x() / float(width())) * 2.0f - 1.0f, 1.0f - (point.y() / float(height())) * 2.0f); 
}
QPointF PiYingGL::mapToGL(float x, float y) const
{ 
	return QPointF((x / float(width())) * 2.0f - 1.0f, 1.0f - (y / float(height())) * 2.0f); 
}

QPointF PiYingGL::glToMap(const QPointF& point) const
{ 
	return QPointF((point.x() + 1.0f) * width() / 2, (1.0f - point.y()) * height() / 2); 
}

QPointF PiYingGL::mapViewProjMatrix(const QPointF& point) const
{ 
	return glToMap(getViewProjMatrix().map(point)); 
}
QPointF PiYingGL::GLViewProjMatrixInvert(float x, float y) const
{ 
	return getViewProjMatrixInvert().map(mapToGL(x, y)); 
}

QPointF PiYingGL::GLViewProjMatrixInvert(const QPointF& point) const
{ 
	return getViewProjMatrixInvert().map(mapToGL(point)); 
}

QPointF PiYingGL::GLViewMatrixInvert(const QPointF& point) const
{
	return getViewMatrixInvert().map(mapToGL(point));
}

QMatrix4x4 PiYingGL::getViewMatrixInvert() const {
	QMatrix4x4 mViewTransform;
	mViewTransform.rotate(-viewRotate.value(), 0.0f, 0.0f, 1.0f);
	mViewTransform.scale(1 / viewScale.value());
	mViewTransform.translate(-viewTransX.value(), -viewTransY.value());
	return mViewTransform;
}

QMatrix4x4 PiYingGL::getViewMatrix() const {
	QMatrix4x4 mViewTransform;
	mViewTransform.translate(viewTransX.value(), viewTransY.value());
	mViewTransform.scale(viewScale.value());
	mViewTransform.rotate(viewRotate.value(), 0.0f, 0.0f, 1.0f);
	return mViewTransform;
}

QMatrix4x4 PiYingGL::getBgShaderMatrix(const QMatrix4x4& transform) const
{ 
	return proj * getViewMatrix() * transform * insProj;
}

QMatrix4x4 PiYingGL::getViewProjMatrixInvert() const
{ 
	return proj * getViewMatrixInvert() * insProj; 
}

QMatrix4x4 PiYingGL::getViewProjMatrix() const
{
	return proj * getViewMatrix() * insProj;
}

QMatrix4x4 PiYingGL::getProj() const 
{
	return proj; 
}

QMatrix4x4 PiYingGL::getInsProj() const 
{
	return insProj;
}