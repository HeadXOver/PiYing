#include "piYingGL.h"

void PiYingGL::bgRotationControl(const QPointF& mouse, ImageTexture& image)
{
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
	QPointF mouseRaletiveStatic = getRaletiveToRect(QPointF(mouse.x(), mouse.y()), LastImageTransform);
	QPointF LastMouseRaletive = getRaletiveToRect(QPointF(LastMousePos.x(), LastMousePos.y()), LastImageTransform);
	QPointF toMoveRaletive = getRaletiveToRectWithoutTrans((mouse - LastMousePos), LastImageTransform) / viewScale;

	image.trans = LastImageTransform.trans;

	if (LastMousePosType == MousePos::BottomEdge) {
		image.setScale(1.f, (1.0f - mouseRaletiveStatic.y()) / (1.0f - LastMouseRaletive.y()));
		toMoveRaletive = getRaletiveToGlobalWithoutTrans(QPointF(0.f, toMoveRaletive.y() / 2.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f - LastMouseRaletive.y()) * 2.f, toMoveRaletive.y() / (1.0f - LastMouseRaletive.y()) * 2.f);
	}
	else if (LastMousePosType == MousePos::TopEdge) {
		image.setScale(1.f, (1.0f + mouseRaletiveStatic.y()) / (1.0f + LastMouseRaletive.y()));
		toMoveRaletive = getRaletiveToGlobalWithoutTrans(QPointF(0.f, toMoveRaletive.y() / 2.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f + LastMouseRaletive.y()) * 2.f, toMoveRaletive.y() / (1.0f + LastMouseRaletive.y()) * 2.f);
	}
	else if (LastMousePosType == MousePos::LeftEdge) {
		image.setScale((1.0f - mouseRaletiveStatic.x()) / (1.0f - LastMouseRaletive.x()), 1.f);
		toMoveRaletive = getRaletiveToGlobalWithoutTrans(QPointF(toMoveRaletive.x() / 2.f, 0.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f - LastMouseRaletive.x()) * 2.f, toMoveRaletive.y() / (1.0f - LastMouseRaletive.x()) * 2.f);
	}
	else if (LastMousePosType == MousePos::RightEdge) {
		image.setScale((1.0f + mouseRaletiveStatic.x()) / (1.0f + LastMouseRaletive.x()), 1.f);
		toMoveRaletive = getRaletiveToGlobalWithoutTrans(QPointF(toMoveRaletive.x() / 2.f, 0.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f + LastMouseRaletive.x()) * 2.f, toMoveRaletive.y() / (1.0f + LastMouseRaletive.x()) * 2.f);
	}
	else if (LastMousePosType == MousePos::LeftTop) {
		image.setScale((1.0f - mouseRaletiveStatic.x()) / (1.0f - LastMouseRaletive.x()), (1.0f + mouseRaletiveStatic.y()) / (1.0f + LastMouseRaletive.y()));
		toMoveRaletive = getRaletiveToGlobalWithoutTrans(QPointF(toMoveRaletive.x() / 2.f, toMoveRaletive.y() / 2.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f - LastMouseRaletive.x()) * 2.f, toMoveRaletive.y() / (1.0f + LastMouseRaletive.y()) * 2.f);
	}
	else if (LastMousePosType == MousePos::LeftBottom) {
		image.setScale((1.0f - mouseRaletiveStatic.x()) / (1.0f - LastMouseRaletive.x()), (1.0f - mouseRaletiveStatic.y()) / (1.0f - LastMouseRaletive.y()));
		toMoveRaletive = getRaletiveToGlobalWithoutTrans(QPointF(toMoveRaletive.x() / 2.f, toMoveRaletive.y() / 2.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f - LastMouseRaletive.x()) * 2.f, toMoveRaletive.y() / (1.0f - LastMouseRaletive.y()) * 2.f);
	}
	else if (LastMousePosType == MousePos::RightTop) {
		image.setScale((1.0f + mouseRaletiveStatic.x()) / (1.0f + LastMouseRaletive.x()), (1.0f + mouseRaletiveStatic.y()) / (1.0f + LastMouseRaletive.y()));
		toMoveRaletive = getRaletiveToGlobalWithoutTrans(QPointF(toMoveRaletive.x() / 2.f, toMoveRaletive.y() / 2.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f + LastMouseRaletive.x()) * 2.f, toMoveRaletive.y() / (1.0f + LastMouseRaletive.y()) * 2.f);
	}
	else if (LastMousePosType == MousePos::RightBottom) {
		image.setScale((1.0f + mouseRaletiveStatic.x()) / (1.0f + LastMouseRaletive.x()), (1.0f - mouseRaletiveStatic.y()) / (1.0f - LastMouseRaletive.y()));
		toMoveRaletive = getRaletiveToGlobalWithoutTrans(QPointF(toMoveRaletive.x() / 2.f, toMoveRaletive.y() / 2.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f + LastMouseRaletive.x()) * 2.f, toMoveRaletive.y() / (1.0f - LastMouseRaletive.y()) * 2.f);
	}


	image.scale *= LastImageTransform.scale;
}