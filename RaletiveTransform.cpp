#include "piYingGL.h"

#include <QMessageBox>

void PiYingGL::raletiveToRect(QPointF& point, const ImageTexture& image)
{
	point = getRaletiveToRect(point, image);
}

void PiYingGL::raletiveToRect(QPointF& point, const ImageTransform& transform)
{
	point = getRaletiveToRect(point, transform);
}

void PiYingGL::raletiveToRectWithoutTrans(QPointF& point, const ImageTransform& transform)
{
	point = getRaletiveToRectWithoutTrans(point, transform);
}

void PiYingGL::raletiveToRect(QVector4D& vec4, const ImageTexture& image)
{
	QMatrix4x4 mViewScale;
	mViewScale.scale(1 / viewScale);
	vec4 = proj * mViewScale * image.scale.inverted() * image.rot.inverted() * image.trans.inverted() * insProj * vec4;
}

void PiYingGL::raletiveToGlobal(QPointF& point, const ImageTexture& image)
{
	point = getRaletiveToGlobal(point, image);
}

void PiYingGL::raletiveToGlobal(QPointF& point, const ImageTransform& transform)
{
	point = getRaletiveToGlobal(point, transform);
}

void PiYingGL::raletiveToGlobalWithoutTrans(QPointF& point, const ImageTransform& transform)
{
	point = getRaletiveToGlobalWithoutTrans(point, transform);
}

QPointF PiYingGL::getRaletiveToRect(const QPointF& point, const ImageTexture& image)
{
	QMatrix4x4 mViewScale;
	mViewScale.scale(1 / viewScale);
	return (proj * mViewScale * image.scale.inverted() * image.rot.inverted() * image.trans.inverted() * insProj).map(point);
}

QPointF PiYingGL::getRaletiveToRect(const QPointF& point, const ImageTransform& transform)
{
	QMatrix4x4 mViewScale;
	mViewScale.scale(1 / viewScale);
	return (proj * mViewScale * transform.scale.inverted() * transform.rot.inverted() * transform.trans.inverted() * insProj).map(point);
}

QPointF PiYingGL::getRaletiveToRectWithoutTrans(const QPointF& point, const ImageTransform& transform)
{
	QMatrix4x4 mViewScale;
	mViewScale.scale(1 / viewScale);
	return (proj * mViewScale * transform.scale.inverted() * transform.rot.inverted() * insProj).map(point);
}

QPointF PiYingGL::getRaletiveToGlobal(const QPointF& point, const ImageTexture& image)
{
	QMatrix4x4 mViewScale;
	mViewScale.scale(viewScale);
	return (proj * image.trans * image.rot * image.scale * mViewScale * insProj).map(point);
}

QPointF PiYingGL::getRaletiveToGlobal(const QPointF& point, const ImageTransform& transform)
{
	QMatrix4x4 mViewScale;
	mViewScale.scale(viewScale);
	return (proj * transform.trans * transform.rot * transform.scale * mViewScale * insProj).map(point);
}

QPointF PiYingGL::getRaletiveToGlobalWithoutTrans(const QPointF& point, const ImageTransform& transform)
{
	QMatrix4x4 mViewScale;
	mViewScale.scale(viewScale);
	return (proj * transform.rot * transform.scale * mViewScale * insProj).map(point);
}
