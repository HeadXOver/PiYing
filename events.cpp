#include "piYingGL.h"
#include "PiYing.h"

void PiYingGL::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
		QPointF mouse = event->position();
		lastMousePos = mapToGL(mouse);

		if (editMode == EditMode::BackGround) {
			lastMousePosType = MousePos::OutSide;
			int cur = getCurrentBgRow();
			if(cur < 0) return;
			ImageTexture& item = backGrounds[cur];
			QPointF posV = getRaletiveToRect(lastMousePos, item.transform);
			if (isInsideSquare(posV)) {
				lastMousePosType = getMousePosType(posV);
				lastImageTransform = item.transform;
			}
		}
		else if (editMode == EditMode::characterTexture && event->button() == Qt::LeftButton) {
			chToolControl(lastMousePos);
		}

		currentUpdate();
	}
	else if (event->button() == Qt::MiddleButton) {
		lastMiddleButtonPos = mapToGL(event->position());
		lastViewTransX = viewTransX.value();
		lastViewTransY = viewTransY.value();
		lastViewRotate = viewRotate.value();
	}
}

void PiYingGL::mouseReleaseEvent(QMouseEvent* e)
{
	if (chElementTool) chElementTool->releasePos(getViewProjMatrixInvert().map(mapToGL(e->position())));

	update();
}

void PiYingGL::mouseMoveEvent(QMouseEvent* event) {
	QPointF mouse = mapToGL(event->position());
	if (event->buttons() == Qt::LeftButton) {
		if (lastMousePosType != MousePos::OutSide) {
			ImageTexture& item = backGrounds[parent->getCurrentBgRow()];
			if (editMode == EditMode::BackGround) {
				if (KeyboardStateWin::isAltHeld())
					bgRotationControl(mouse, item);
				else if (lastMousePosType == MousePos::Inside)
					bgTranslateControl(mouse, item);
				else
					bgScaleControl(mouse, item);

				currentUpdate();
			}
		}
		else if (editMode == EditMode::characterTexture) {
			if (chElementTool) {
				chElementTool->movePos(getViewProjMatrixInvert().map(mouse));
				currentUpdate();
			}
		}
	}
	else if (event->buttons() == Qt::MiddleButton) {
		if (KeyboardStateWin::isAltHeld()) viewRotationControl(mouse);
		else {
			QPointF toTrans = insProj.map(mouse - lastMiddleButtonPos);
			viewTransX.setValue(lastViewTransX + toTrans.x());
			viewTransY.setValue(lastViewTransY + toTrans.y());
		}
		currentUpdate();
	}
	else {
		int cur = parent->getCurrentBgRow();
		if (cur >= 0) setCursor(getCursorShape(getMousePosType(getRaletiveToRect(mouse, backGrounds[cur].transform))));
		else setCursor(Qt::CursorShape::ArrowCursor);
	}
}

void PiYingGL::wheelEvent(QWheelEvent* ev) {
	float scaleFactor = 1.0f + ev->angleDelta().y() / 1200.f;
	if (scaleFactor < 0.1f) scaleFactor = 0.1f;

	QPointF toTrans = insProj.map(mapToGL(ev->position()) * (1 - scaleFactor));
	viewScale.setValue(viewScale.value() * scaleFactor);
	viewTransX.setValue(viewTransX.value() * scaleFactor + toTrans.x());
	viewTransY.setValue(viewTransY.value() * scaleFactor + toTrans.y());

	currentUpdate();
	ev->accept();
}

void PiYingGL::contextMenuEvent(QContextMenuEvent* e)
{
	if (editMode == EditMode::characterTexture)  rightButtonMenuChTex->exec(e->globalPos());
	else if (editMode == EditMode::BackGround) {
		if (lastMousePosType != MousePos::OutSide) rightButtonMenuBg_S->exec(e->globalPos());
		else rightButtonMenuBg->exec(e->globalPos());
	}
	else rightButtonMenu->exec(e->globalPos());

	e->accept();
}
