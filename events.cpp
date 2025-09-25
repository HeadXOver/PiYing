#include "piYingGL.h"

void PiYingGL::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
		QPointF mouse = event->position();
		lastMousePos = mapToGL(mouse);

		if (editMode == EditMode::BackGround) {
			currentSelectedBackGround = -1;
			lastMousePosType = MousePos::OutSide;
			for (int i = backGrounds.size() - 1; i >= 0; i--) {
				ImageTexture& item = backGrounds[i];
				QPointF posV = getRaletiveToRect(lastMousePos, item.transform);
				if (isInsideSquare(posV)) {
					lastMousePosType = getMousePosType(posV);
					lastImageTransform = item.transform;
					currentSelectedBackGround = i;
					break;
				}
			}
		}
		else if (editMode == EditMode::characterTexture && event->button() == Qt::LeftButton) {
			if(toolState == ToolState::AddVert) chEditVertControl(mouse);
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
			viewTransX.setValue(lastViewTransX + toTrans.x());
			viewTransY.setValue(lastViewTransY + toTrans.y());
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
		QPointF mouse = mapToGL(ev->position());
		int delta = numSteps.y();
		float scaleFactor = 1.0f + delta * 0.1f;
		if (scaleFactor < 0.1f) scaleFactor = 0.1f;

		QPointF toTrans = insProj.map(mouse + (QPointF(0.f, 0.f) - mouse) * scaleFactor);
		viewScale.setValue(viewScale.value() * scaleFactor);
		viewTransX.setValue(viewTransX.value() * scaleFactor + toTrans.x());
		viewTransY.setValue(viewTransY.value() * scaleFactor + toTrans.y());

		currentUpdate();
	}

	ev->accept();
}

void PiYingGL::contextMenuEvent(QContextMenuEvent* e)
{
	QMenu menu(this);

	if (editMode == EditMode::characterTexture) {
		menu.addAction(actionAddCharacterTexture);
	}
	else if (editMode == EditMode::BackGround) {
		if (currentSelectedBackGround >= 0) {
			menu.addAction(actionFullScreenBackGround);
			menu.addAction(actionDeleteBg);
			menu.addAction(actionBgSetTransform);
			menu.addAction(actionAgainstBg);
			menu.addAction(actionReturnbgTransform);
		}
		else {
			menu.addAction(actionAddBackGround);
			menu.addAction(actionDeleteAllBg);
		}
	}
	menu.addAction(actionChoseBackGroundColor);
	menu.addAction(actionSetViewToStandard);
	menu.addAction(actionReturnToStandard);

	menu.exec(e->globalPos());
	e->accept();
}
