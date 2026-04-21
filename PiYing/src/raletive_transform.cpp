#include "piYingGL.h"

#include "image_transform.h"

QPointF PiYingGL::getRaletiveToRect(const QPointF& point, const QMatrix4x4& transform) const
{
	return (_orth_ratio * transform * _view_matrix_invert * _orth_ratio_invert).map(point);
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
	return glToMap(_view_proj_matrix.map(point));
}
QPointF PiYingGL::GLViewProjMatrixInvert(float x, float y) const
{ 
	return _view_proj_matrix_invert.map(mapToGL(x, y));
}

QPointF PiYingGL::GLViewProjMatrixInvert(const QPointF& point) const
{ 
	return _view_proj_matrix_invert.map(mapToGL(point));
}

QPointF PiYingGL::GLViewMatrixInvert(const QPointF& point) const
{
	return _view_matrix_invert.map(mapToGL(point));
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

	update_view_proj_matrix();
}

void PiYingGL::update_view_proj_matrix()
{
	_view_proj_matrix = _orth_ratio * _view_matrix * _orth_ratio_invert;
	_view_proj_matrix_invert = _orth_ratio * _view_matrix_invert * _orth_ratio_invert;
}

QMatrix4x4 PiYingGL::getBgShaderMatrix(const QMatrix4x4& transform) const noexcept
{ 
	return _orth_ratio * _view_matrix * transform * _orth_ratio_invert;
}