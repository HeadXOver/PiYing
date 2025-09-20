#include "piYingGL.h"

void PiYingGL::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		LastMousePos = mapToGL(event->position());

		currentSelectedBackGround = -1;
		LastMousePosType = MousePos::OutSide;

		for (ImageTexture& item : backGrounds)  item.selected = false;
		if (editMode == EditMode::BackGround) {
			for (int i = backGrounds.size() - 1; i >= 0; i--) {
				ImageTexture& item = backGrounds[i];
				QPointF posV = getRaletiveToRect(QPointF(LastMousePos.x(), LastMousePos.y()), item);
				if (isInsideSquare(posV)) {
					LastMousePosType = getMousePosType(posV, item);
					LastImageTransform = { item.trans, item.rot, item.scale };
					item.selected = true;
					currentSelectedBackGround = i;
					break;
				}
			}
		}
		currentUpdate();
	}
	else if (event->button() == Qt::MiddleButton) {
		LastMiddleButtonPos = mapToGL(event->position());
	}
}

void PiYingGL::mouseMoveEvent(QMouseEvent* event) {
	if (currentSelectedBackGround >= 0) {
		ImageTexture& item = backGrounds[currentSelectedBackGround];
		QPointF mouse = mapToGL(event->position());
		if (event->buttons() == Qt::LeftButton) {
			if (editMode == EditMode::BackGround) {
				if (KeyboardStateWin::isAltHeld())
					bgRotationControl(mouse, item);
				else if (LastMousePosType == MousePos::Inside)
					bgTranslateControl(mouse, item);
				else
					bgScaleControl(mouse, item);

				currentUpdate();
			}
		}
		else {
			raletiveToRect(mouse, item);
			setCursor(getCursorShape(getMousePosType(mouse, item)));
		}
	}
	else {
		setCursor(Qt::ArrowCursor);
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
