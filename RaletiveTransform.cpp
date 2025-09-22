#include "piYingGL.h"

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

QMatrix4x4 PiYingGL::getViewMatrixInvertWithoutTrans() const
{
	QMatrix4x4 mViewTransform;
	mViewTransform.rotate(-viewRotate.value(), 0.0f, 0.0f, 1.0f);
	mViewTransform.scale(1 / viewScale.value());
	return mViewTransform;
}

QMatrix4x4 PiYingGL::getViewMatrixWithoutTrans() const
{
	QMatrix4x4 mViewTransform;
	mViewTransform.scale(viewScale.value());
	mViewTransform.rotate(viewRotate.value(), 0.0f, 0.0f, 1.0f);
	return mViewTransform;
}

QMatrix4x4 PiYingGL::getBgShaderMatrix(const ImageTransform& transform) const
{
	return proj * getViewMatrix() * transform.getMatrix() * insProj;
}