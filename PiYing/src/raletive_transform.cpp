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

void PiYingGL::update_view_matrix()
{
	_view_matrix = {
		viewScale.value(), 0.0f, 0.0f, viewTransX.value(),
		0.0f, viewScale.value(), 0.0f, viewTransY.value(),
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	_view_matrix.rotate(viewRotate.value(), 0.0f, 0.0f, 1.0f);

	_view_matrix_invert.setToIdentity();
	_view_matrix_invert.rotate(-viewRotate.value(), 0.0f, 0.0f, 1.0f);

	float scale = 1 / viewScale.value();

	_view_matrix_invert.scale(scale, scale);
	_view_matrix_invert.translate(-viewTransX.value(), -viewTransY.value());
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

const QMatrix4x4& PiYingGL::getProj() const noexcept
{
	return _orth_ratio;
}

const QMatrix4x4& PiYingGL::getInsProj() const noexcept
{
	return _orth_ratio_invert;
}