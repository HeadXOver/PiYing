#include "piYingGL.h"

void PiYingGL::bgRotationControl(const QPointF& mouse, ImageTexture& image)
{
	setCursor(Qt::CursorShape::ClosedHandCursor);
	QPointF center = (getViewMatrix() * lastImageTransform.trans).map(QPointF(0.f, 0.f));
	center.setX(center.x() / aspect);
	QPointF vec1 = lastMousePos - center;
	QPointF vec2 = mouse - center;
	float r = (atan2f(vec2.y(), vec2.x()) - atan2f(vec1.y(), vec1.x())) * 180.f / 3.1415926f;

	image.transform = lastImageTransform;
	image.addRot(r);
}

void PiYingGL::bgTranslateControl(const QPointF& mouse, ImageTexture& image)
{
	setCursor(Qt::ClosedHandCursor);
	image.transform = lastImageTransform;
	image.addTrans((getViewMatrixInvertWithoutTrans() * insProj).map(QPointF(mouse.x() - lastMousePos.x(), mouse.y() - lastMousePos.y())));
}

void PiYingGL::bgScaleControl(const QPointF& mouse, ImageTexture& image)
{
	QPointF mouseRaletive = getRaletiveToRect(mouse, lastImageTransform);
	QPointF LastMouseRaletive = getRaletiveToRect(lastMousePos, lastImageTransform);
	QPointF pAspect(1.0f, 1.0f);
	short PN[2] = { 1, 1 };

	image.transform = lastImageTransform;

	if (lastMousePosType == MousePos::BottomEdge || lastMousePosType == MousePos::LeftBottom || lastMousePosType == MousePos::RightBottom) {
		pAspect.setY((1.0f - mouseRaletive.y()) / (1.0f - LastMouseRaletive.y()));
	}
	else if (lastMousePosType == MousePos::TopEdge || lastMousePosType == MousePos::LeftTop || lastMousePosType == MousePos::RightTop) {
		pAspect.setY((1.0f + mouseRaletive.y()) / (1.0f + LastMouseRaletive.y()));
		PN[0] = -1;
	}
	if (lastMousePosType == MousePos::LeftEdge || lastMousePosType == MousePos::LeftTop || lastMousePosType == MousePos::LeftBottom) {
		pAspect.setX((1.0f - mouseRaletive.x()) / (1.0f - LastMouseRaletive.x()));
	}
	else if (lastMousePosType == MousePos::RightEdge || lastMousePosType == MousePos::RightBottom || lastMousePosType == MousePos::RightTop) {
		pAspect.setX((1.0f + mouseRaletive.x()) / (1.0f + LastMouseRaletive.x()));
		PN[1] = -1;
	}

	image.addScale(pAspect);
	pAspect.setX(PN[1] * (1.f - pAspect.x()));
	pAspect.setY(PN[0] * (1.f - pAspect.y()));
	image.addTrans((lastImageTransform.rot * lastImageTransform.scale * insProj).map(pAspect));
}

void PiYingGL::viewRotationControl(const QPointF& mouse)
{
	setCursor(Qt::CursorShape::ClosedHandCursor);
	float r = (atan2f(mouse.y(), mouse.x()) - atan2f(lastMiddleButtonPos.y(), lastMiddleButtonPos.x())) * 180.f / 3.1415926f;
	viewRotate = lastViewRotate + r;

	QMatrix4x4 toTrans;
	toTrans.rotate(r, 0.f, 0.0f, 1.0f);
	QPointF viewTrans(lastViewTransX, lastViewTransY);
	QPointF rotatedTrans = toTrans.map(viewTrans) - viewTrans;
	viewTransX = lastViewTransX + rotatedTrans.x();
	viewTransY = lastViewTransY + rotatedTrans.y();
}
