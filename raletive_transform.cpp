#include "piYingGL.h"

#include "image_transform.h"

QPointF PiYingGL::getRaletiveToRect(const QPointF& point, const QMatrix4x4& transform) const
{
	return (_orth_ratio * transform * getViewMatrixInvert() * _orth_ratio_invert).map(point);
}

QPointF PiYingGL::mapToGL(const QPointF& point) const
{ 
	return QPointF((point.x() / static_cast<float>(width())) * 2.0f - 1.0f, 1.0f - (point.y() / static_cast<float>(height())) * 2.0f); 
}
QPointF PiYingGL::mapToGL(float x, float y) const
{ 
	return QPointF((x / static_cast<float>(width())) * 2.0f - 1.0f, 1.0f - (y / static_cast<float>(height())) * 2.0f);
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

QMatrix4x4 PiYingGL::getViewMatrixInvert() const noexcept 
{
	QMatrix4x4 mViewTransform;
	mViewTransform.rotate(-viewRotate.value(), 0.0f, 0.0f, 1.0f);
	mViewTransform.scale(1 / viewScale.value());
	mViewTransform.translate(-viewTransX.value(), -viewTransY.value());
	return mViewTransform;
}

QMatrix4x4 PiYingGL::getViewMatrix() const noexcept
{
	QMatrix4x4 mViewTransform;
	mViewTransform.translate(viewTransX.value(), viewTransY.value());
	mViewTransform.scale(viewScale.value());
	mViewTransform.rotate(viewRotate.value(), 0.0f, 0.0f, 1.0f);
	return mViewTransform;
}

QMatrix4x4 PiYingGL::getBgShaderMatrix(const QMatrix4x4& transform) const noexcept
{ 
	return _orth_ratio * getViewMatrix() * transform * _orth_ratio_invert;
}

QMatrix4x4 PiYingGL::getViewProjMatrixInvert() const noexcept
{ 
	return _orth_ratio * getViewMatrixInvert() * _orth_ratio_invert;
}

QMatrix4x4 PiYingGL::getViewProjMatrix() const noexcept
{
	return _orth_ratio * getViewMatrix() * _orth_ratio_invert;
}

QMatrix4x4 PiYingGL::getProj() const noexcept
{
	return _orth_ratio;
}

QMatrix4x4 PiYingGL::getInsProj() const noexcept
{
	return _orth_ratio_invert;
}