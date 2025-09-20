#include "piYingGL.h"

void PiYingGL::bgRotationControl(const QPointF& mouse, ImageTexture& image)
{
	setCursor(Qt::CursorShape::ClosedHandCursor);
	QPointF center = LastImageTransform.trans.map(QPointF(0.f, 0.f)) * viewScale;
	center.setX(center.x() / aspect);
	QPointF vec1 = LastMousePos - center;
	QPointF vec2 = mouse - center;
	float r = (atan2f(vec2.y(), vec2.x()) - atan2f(vec1.y(), vec1.x())) * 180.f / 3.1415926f;

	image.setRot(r);
	image.rot *= LastImageTransform.rot;
}

void PiYingGL::bgTranslateControl(const QPointF& mouse, ImageTexture& image)
{
	setCursor(Qt::ClosedHandCursor);
	image.trans = LastImageTransform.trans;
	image.addTrans(insProj.map(QPointF(mouse.x() - LastMousePos.x(), mouse.y() - LastMousePos.y()) / viewScale));
}

void PiYingGL::bgScaleControl(const QPointF& mouse, ImageTexture& image)
{
	QPointF mouseRaletive = getRaletiveToRect(mouse, LastImageTransform);
	QPointF LastMouseRaletive = getRaletiveToRect(LastMousePos, LastImageTransform);
	QPointF toMove = (mouse - LastMousePos) / viewScale;
	QPointF toMoveRaletive = getRaletiveToRectWithoutTrans(toMove, LastImageTransform);

	image.trans = LastImageTransform.trans;

	if (LastMousePosType == MousePos::BottomEdge) {
		image.setScale(1.f, (1.0f - mouseRaletive.y()) / (1.0f - LastMouseRaletive.y()));
		toMoveRaletive.setX(0.f);
		image.addTrans(getRaletiveToGlobalWithoutTransProj(toMoveRaletive, LastImageTransform) / (1.0f - LastMouseRaletive.y()));
	}
	else if (LastMousePosType == MousePos::TopEdge) {
		image.setScale(1.f, (1.0f + mouseRaletive.y()) / (1.0f + LastMouseRaletive.y()));
		toMoveRaletive.setX(0.f);
		image.addTrans(getRaletiveToGlobalWithoutTransProj(toMoveRaletive, LastImageTransform) / (1.0f + LastMouseRaletive.y()));
	}
	else if (LastMousePosType == MousePos::LeftEdge) {
		image.setScale((1.0f - mouseRaletive.x()) / (1.0f - LastMouseRaletive.x()), 1.f);
		toMoveRaletive.setY(0.f);
		image.addTrans(getRaletiveToGlobalWithoutTransProj(toMoveRaletive, LastImageTransform) / (1.0f - LastMouseRaletive.x()));
	}
	else if (LastMousePosType == MousePos::RightEdge) {
		image.setScale((1.0f + mouseRaletive.x()) / (1.0f + LastMouseRaletive.x()), 1.f);
		toMoveRaletive.setY(0.f);
		image.addTrans(getRaletiveToGlobalWithoutTransProj(toMoveRaletive, LastImageTransform) / (1.0f + LastMouseRaletive.x()));
	}
	else {
		toMove = insProj.map(toMove);

		if (LastMousePosType == MousePos::LeftTop) {
			image.setScale((1.0f - mouseRaletive.x()) / (1.0f - LastMouseRaletive.x()), (1.0f + mouseRaletive.y()) / (1.0f + LastMouseRaletive.y()));
			image.trans.translate(toMove.x() / (1.0f - LastMouseRaletive.x()), toMove.y() / (1.0f + LastMouseRaletive.y()));
		}
		else if (LastMousePosType == MousePos::LeftBottom) {
			image.setScale((1.0f - mouseRaletive.x()) / (1.0f - LastMouseRaletive.x()), (1.0f - mouseRaletive.y()) / (1.0f - LastMouseRaletive.y()));
			image.trans.translate(toMove.x() / (1.0f - LastMouseRaletive.x()), toMove.y() / (1.0f - LastMouseRaletive.y()));
		}
		else if (LastMousePosType == MousePos::RightTop) {
			image.setScale((1.0f + mouseRaletive.x()) / (1.0f + LastMouseRaletive.x()), (1.0f + mouseRaletive.y()) / (1.0f + LastMouseRaletive.y()));
			image.trans.translate(toMove.x() / (1.0f + LastMouseRaletive.x()), toMove.y() / (1.0f + LastMouseRaletive.y()));
		}
		else if (LastMousePosType == MousePos::RightBottom) {
			image.setScale((1.0f + mouseRaletive.x()) / (1.0f + LastMouseRaletive.x()), (1.0f - mouseRaletive.y()) / (1.0f - LastMouseRaletive.y()));
			image.trans.translate(toMove.x() / (1.0f + LastMouseRaletive.x()), toMove.y() / (1.0f - LastMouseRaletive.y()));
		}
	}

	image.scale *= LastImageTransform.scale;
}