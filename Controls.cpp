#include "piYingGL.h"
#include "piYing.h"
#include "cus_func_point2d.h"
#include "image_transform.h"
#include "KeyboardStateWin.h"
#include "image_texture.h"

namespace {
	constexpr float angle_rad = 3.1415926f / 180.f;
}

void PiYingGL::bgRotationControl(const QPointF& mouse, ImageTexture& image)
{
	setCursor(Qt::CursorShape::ClosedHandCursor);
	QPointF center = getBgShaderMatrix(lastImageTransform->getMatrix()).map(QPointF(0.f, 0.f));
	QPointF vec1 = _orth_ratio_invert.map(lastMousePos - center);
	QPointF vec2 = _orth_ratio_invert.map(mouse - center);
	image.set_transform(*lastImageTransform);
	image.addRot(PiYingCus::angleBetweenPointDegree(vec2, vec1));
}

void PiYingGL::bgTranslateControl(const QPointF& mouse, ImageTexture& image)
{
	setCursor(Qt::ClosedHandCursor);
	image.set_transform(*lastImageTransform);
	image.addTrans(PiYingCus::getRotatedPoint(_orth_ratio_invert.map(mouse - lastMousePos) / viewScale.value(), -viewRotate.value() * angle_rad));
}

void PiYingGL::bgScaleControl(const QPointF& mouse, ImageTexture& image)
{
	const QPointF mouseRaletive = getRaletiveToRect(mouse, lastImageTransform->getMatrixInvert());
	const QPointF LastMouseRaletive = getRaletiveToRect(lastMousePos, lastImageTransform->getMatrixInvert());
	QPointF pAspect(1.0f, 1.0f);

	image.set_transform(*lastImageTransform);

	short topButton = 1;
	if (lastMousePosType == MousePos::BottomEdge || lastMousePosType == MousePos::LeftBottom || lastMousePosType == MousePos::RightBottom) {
		pAspect.setY((1.0f - mouseRaletive.y()) / (1.0f - LastMouseRaletive.y()));
	}
	else if (lastMousePosType == MousePos::TopEdge || lastMousePosType == MousePos::LeftTop || lastMousePosType == MousePos::RightTop) {
		pAspect.setY((1.0f + mouseRaletive.y()) / (1.0f + LastMouseRaletive.y()));
		topButton = -1;
	}

	short leftRight = 1;
	if (lastMousePosType == MousePos::LeftEdge || lastMousePosType == MousePos::LeftTop || lastMousePosType == MousePos::LeftBottom) {
		pAspect.setX((1.0f - mouseRaletive.x()) / (1.0f - LastMouseRaletive.x()));
	}
	else if (lastMousePosType == MousePos::RightEdge || lastMousePosType == MousePos::RightBottom || lastMousePosType == MousePos::RightTop) {
		pAspect.setX((1.0f + mouseRaletive.x()) / (1.0f + LastMouseRaletive.x()));
		leftRight = -1;
	}

	if (KeyboardStateWin::isShiftHeld()) {
		pAspect.setY(pAspect.x());
	}

	image.addScale(pAspect);
	pAspect.setX(leftRight * (1.f - pAspect.x()));
	pAspect.setY(topButton * (1.f - pAspect.y()));
	image.addTrans((lastImageTransform->rot() * lastImageTransform->scale() * _orth_ratio_invert).map(pAspect));
}

void PiYingGL::viewRotationControl(const QPointF& mouse)
{
	using namespace PiYingCus;

	setCursor(Qt::CursorShape::ClosedHandCursor);
	float r = angleBetweenPoint(_orth_ratio_invert.map(mouse), _orth_ratio_invert.map(lastMiddleButtonPos));
	viewRotate.setValue(lastViewRotate + r * 180.f / 3.1415926f);
	QPointF toRot = getRotatedPoint(lastViewTransX, lastViewTransY, r);
	viewTransX.setValue(toRot.x());
	viewTransY.setValue(toRot.y());
	update_trc();
}