#include "piYingGL.h"

void PiYingGL::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
		lastMousePos = mapToGL(event->position());

		currentSelectedBackGround = -1;
		lastMousePosType = MousePos::OutSide;

		if (editMode == EditMode::BackGround) {
			for (int i = backGrounds.size() - 1; i >= 0; i--) {
				ImageTexture& item = backGrounds[i];
				QPointF posV = getRaletiveToRect(QPointF(lastMousePos.x(), lastMousePos.y()), item.transform);
				if (isInsideSquare(posV)) {
					lastMousePosType = getMousePosType(posV);
					lastImageTransform = item.transform;
					currentSelectedBackGround = i;
					break;
				}
			}
		}

		currentUpdate();
	}
	else if (event->button() == Qt::MiddleButton) {
		lastMiddleButtonPos = mapToGL(event->position());
		lastViewTransX = viewTransX;
		lastViewTransY = viewTransY;
		lastViewRotate = viewRotate;
	}
}

void PiYingGL::mouseMoveEvent(QMouseEvent* event) {
	QPointF mouse = mapToGL(event->position());
	if (event->buttons() == Qt::LeftButton) {
		if (currentSelectedBackGround >= 0) {
			ImageTexture& item = backGrounds[currentSelectedBackGround];
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
	}
	else if (event->buttons() == Qt::MiddleButton) {
		if (KeyboardStateWin::isAltHeld()) viewRotationControl(mouse);
		else {
			QPointF toTrans = insProj.map(mouse - lastMiddleButtonPos);
			viewTransX = lastViewTransX + toTrans.x();
			viewTransY = lastViewTransY + toTrans.y();
		}
		currentUpdate();
	}
	else if (currentSelectedBackGround >= 0) {
		ImageTexture& item = backGrounds[currentSelectedBackGround];
		setCursor(getCursorShape(getMousePosType(getRaletiveToRect(mouse, item.transform))));
	}
	else {
		setCursor(Qt::CursorShape::ArrowCursor);
	}
}

void PiYingGL::wheelEvent(QWheelEvent* ev) {
	QPoint numSteps = ev->angleDelta() / 120.f;

	if (!numSteps.isNull()) {
		int delta = numSteps.y();
		float scaleFactor = 1.0f + delta * 0.1f;
		if (scaleFactor < 0.1f) scaleFactor = 0.1f;
		viewScale *= scaleFactor;

		currentUpdate();
	}

	ev->accept();   // self event
}

void PiYingGL::contextMenuEvent(QContextMenuEvent* e)
{
	QMenu menu(this);
	if (currentSelectedBackGround < 0) {
		menu.addAction(actionAddBackGround);
		menu.addAction(actionChoseBackGroundColor);
		menu.addAction(actionSetViewToStandard);
		menu.addAction(actionDeleteAllBg);
		menu.addAction(actionReturnToStandard);
	}
	else {
		menu.addAction(actionFullScreenBackGround);
		menu.addAction(actionDeleteBg);
	}
	menu.exec(e->globalPos());
	e->accept();
}
