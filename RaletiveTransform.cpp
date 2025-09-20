#include "piYingGL.h"

#include <QMessageBox>

QMatrix4x4 PiYingGL::getViewMatrixInvert() const {
	QMatrix4x4 mViewTransform;
	mViewTransform.scale(1 / viewScale);
	return mViewTransform;
}

QMatrix4x4 PiYingGL::getViewMatrix() const {
	QMatrix4x4 mViewTransform;
	mViewTransform.scale(viewScale);
	return mViewTransform;
}
