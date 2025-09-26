#include "piYingGL.h"
#include "piYing.h"

void PiYingGL::bgRotationControl(const QPointF& mouse, ImageTexture& image)
{
	setCursor(Qt::CursorShape::ClosedHandCursor);
	QPointF center = getBgShaderMatrix(lastImageTransform).map(QPointF(0.f, 0.f));
	QPointF vec1 = insProj.map(lastMousePos - center);
	QPointF vec2 = insProj.map(mouse - center);
	image.transform = lastImageTransform;
	image.addRot(angleBetweenPointDegree(vec2, vec1));
}

void PiYingGL::bgTranslateControl(const QPointF& mouse, ImageTexture& image)
{
	setCursor(Qt::ClosedHandCursor);
	image.transform = lastImageTransform;
	image.addTrans(getRotatedPoint(insProj.map(mouse - lastMousePos) / viewScale.value(), -viewRotate.value() * 3.1415926f / 180.f));
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
	if (KeyboardStateWin::isShiftHeld()) {
		pAspect.setY(pAspect.x());
	}
	image.addScale(pAspect);
	pAspect.setX(PN[1] * (1.f - pAspect.x()));
	pAspect.setY(PN[0] * (1.f - pAspect.y()));
	image.addTrans((lastImageTransform.rot * lastImageTransform.scale * insProj).map(pAspect));
}

void PiYingGL::viewRotationControl(const QPointF& mouse)
{
	setCursor(Qt::CursorShape::ClosedHandCursor);
	float r = angleBetweenPoint(insProj.map(mouse), insProj.map(lastMiddleButtonPos));
	viewRotate.setValue(lastViewRotate + r * 180.f / 3.1415926f);
	QPointF toRot = getRotatedPoint(lastViewTransX, lastViewTransY, r);
	viewTransX.setValue(toRot.x());
	viewTransY.setValue(toRot.y());
}

void PiYingGL::chToolControl(const QPointF& origMouse)
{
	int currentVector = parent->chImageList->currentRow();
	if (currentVector < 0) return;

	QPointF mouse = getViewProjMatrixInvert().map(mapToGL(origMouse));
	
	if(chElementTool) chElementTool->clickPos(mouse, viewScale.value(), currentVector);
}