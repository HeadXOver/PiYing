#include "piYingGL.h"
#include "piYing.h"
#include "cus_func_point2d.h"
#include "image_transform.h"
#include "KeyboardStateWin.h"
#include "image_texture.h"

namespace {
	constexpr float angle_rad = 3.1415926f / 180.f;

	constexpr short PN1(MousePos type) {
		if (type == MousePos::TopEdge || type == MousePos::LeftTop || type == MousePos::RightTop) {
			return -1;
		}
		return 1;
	}

	constexpr short PN2(MousePos type) {
		if (type == MousePos::RightEdge || type == MousePos::RightBottom || type == MousePos::RightTop) {
			return -1;
		}
		return 1;
	}
}

void PiYingGL::bgRotationControl(const QPointF& mouse, ImageTexture* image)
{
	setCursor(Qt::CursorShape::ClosedHandCursor);
	QPointF center = getBgShaderMatrix(lastImageTransform).map(QPointF(0.f, 0.f));
	QPointF vec1 = insProj.map(lastMousePos - center);
	QPointF vec2 = insProj.map(mouse - center);
	*image = lastImageTransform;
	image->addRot(angleBetweenPointDegree(vec2, vec1));
}

void PiYingGL::bgTranslateControl(const QPointF& mouse, ImageTexture* image)
{
	setCursor(Qt::ClosedHandCursor);
	*image = lastImageTransform;
	image->addTrans(getRotatedPoint(insProj.map(mouse - lastMousePos) / viewScale.value(), -viewRotate.value() * angle_rad));
}

void PiYingGL::bgScaleControl(const QPointF& mouse, ImageTexture* image)
{
	const QPointF mouseRaletive = getRaletiveToRect(mouse, lastImageTransform);
	const QPointF LastMouseRaletive = getRaletiveToRect(lastMousePos, lastImageTransform);
	QPointF pAspect(1.0f, 1.0f);

	*image = lastImageTransform;

	if (lastMousePosType == MousePos::BottomEdge || lastMousePosType == MousePos::LeftBottom || lastMousePosType == MousePos::RightBottom) {
		pAspect.setY((1.0f - mouseRaletive.y()) / (1.0f - LastMouseRaletive.y()));
	}
	else if (lastMousePosType == MousePos::TopEdge || lastMousePosType == MousePos::LeftTop || lastMousePosType == MousePos::RightTop) {
		pAspect.setY((1.0f + mouseRaletive.y()) / (1.0f + LastMouseRaletive.y()));
	}

	if (lastMousePosType == MousePos::LeftEdge || lastMousePosType == MousePos::LeftTop || lastMousePosType == MousePos::LeftBottom) {
		pAspect.setX((1.0f - mouseRaletive.x()) / (1.0f - LastMouseRaletive.x()));
	}
	else if (lastMousePosType == MousePos::RightEdge || lastMousePosType == MousePos::RightBottom || lastMousePosType == MousePos::RightTop) {
		pAspect.setX((1.0f + mouseRaletive.x()) / (1.0f + LastMouseRaletive.x()));
	}

	if (KeyboardStateWin::isShiftHeld()) {
		pAspect.setY(pAspect.x());
	}

	image->addScale(pAspect);
	pAspect.setX(PN2(lastMousePosType) * (1.f - pAspect.x()));
	pAspect.setY(PN1(lastMousePosType) * (1.f - pAspect.y()));
	image->addTrans((lastImageTransform->get_rot() * lastImageTransform->get_scale() * insProj).map(pAspect));
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