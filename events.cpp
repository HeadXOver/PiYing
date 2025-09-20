#include "piYingGL.h"

void PiYingGL::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		lastMousePos = mapToGL(event->position());

		currentSelectedBackGround = -1;
		lastMousePosType = MousePos::OutSide;

		for (ImageTexture& item : backGrounds)  item.selected = false;
		if (editMode == EditMode::BackGround) {
			for (int i = backGrounds.size() - 1; i >= 0; i--) {
				ImageTexture& item = backGrounds[i];
				QPointF posV = getRaletiveToRect(QPointF(lastMousePos.x(), lastMousePos.y()), item);
				if (isInsideSquare(posV)) {
					lastMousePosType = getMousePosType(posV, item);
					lastImageTransform = { item.trans, item.rot, item.scale };
					item.selected = true;
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
		if (KeyboardStateWin::isAltHeld()) {
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
		else {
			QPointF toTrans = insProj.map(mouse - lastMiddleButtonPos);
			viewTransX = lastViewTransX + toTrans.x();
			viewTransY = lastViewTransY + toTrans.y();
		}
		currentUpdate();
	}
	else if (currentSelectedBackGround >= 0) {
		ImageTexture& item = backGrounds[currentSelectedBackGround];
		QPointF mouse = mapToGL(event->position());
		raletiveToRect(mouse, item);
		setCursor(getCursorShape(getMousePosType(mouse, item)));
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
		menu.addAction(actionReturnToStandard);
	}
	else {
		menu.addAction(actionFullScreenBackGround);
	}
	menu.exec(e->globalPos());
	e->accept();
}
