#include "piYingGL.h"

#include <QMessageBox>

QMatrix4x4 PiYingGL::getViewMatrixInvert() const {
	QMatrix4x4 mViewTransform;
	mViewTransform.rotate(-viewRotate, 0.0f, 0.0f, 1.0f);
	mViewTransform.scale(1 / viewScale);
	mViewTransform.translate(-viewTransX, -viewTransY);
	return mViewTransform;
}

QMatrix4x4 PiYingGL::getViewMatrix() const {
	QMatrix4x4 mViewTransform;
	mViewTransform.translate(viewTransX, viewTransY);
	mViewTransform.scale(viewScale);
	mViewTransform.rotate(viewRotate, 0.0f, 0.0f, 1.0f);
	return mViewTransform;
}

QMatrix4x4 PiYingGL::getViewMatrixInvertWithoutTrans() const
{
	QMatrix4x4 mViewTransform;
	mViewTransform.rotate(-viewRotate, 0.0f, 0.0f, 1.0f);
	mViewTransform.scale(1 / viewScale);
	return mViewTransform;
}

QMatrix4x4 PiYingGL::getViewMatrixWithoutTrans() const
{
	QMatrix4x4 mViewTransform;
	mViewTransform.scale(viewScale);
	mViewTransform.rotate(viewRotate, 0.0f, 0.0f, 1.0f);
	return mViewTransform;
}

QMatrix4x4 PiYingGL::getBgShaderMatrix(const ImageTransform& transform) const
{
	return proj * getViewMatrix() * transform.getMatrix() * insProj;
}