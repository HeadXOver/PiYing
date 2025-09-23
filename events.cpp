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
		else if (editMode == EditMode::characterOverView && event->button() == Qt::LeftButton) {
			if (first2Vert.index == 0) {
				first2Vert.vert[0] = mouse;
				first2Vert.index++;
			}
			else if (first2Vert.index == 1) {
				first2Vert.vert[1] = mouse;
				first2Vert.index++;
			}
			else {
				first2Vert.index = 0;
				first2Vert.vert[0] = mapToGL(first2Vert.vert[0]);
				first2Vert.vert[1] = mapToGL(first2Vert.vert[1]);
				characterTriangleIndices.push_back((unsigned int)characterVerts.size() / 2);
				characterVerts.push_back(first2Vert.vert[0].x());
				characterVerts.push_back(first2Vert.vert[0].y());
				characterTriangleIndices.push_back((unsigned int)characterVerts.size() / 2);
				characterVerts.push_back(first2Vert.vert[1].x());
				characterVerts.push_back(first2Vert.vert[1].y());
				characterTriangleIndices.push_back((unsigned int)characterVerts.size() / 2);
				characterVerts.push_back(lastMousePos.x());
				characterVerts.push_back(lastMousePos.y());
			}
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
		int delta = numSteps.y();
		float scaleFactor = 1.0f + delta * 0.1f;
		if (scaleFactor < 0.1f) scaleFactor = 0.1f;
		viewScale.setValue(viewScale.value() * scaleFactor);

		currentUpdate();
	}

	ev->accept();
}

void PiYingGL::contextMenuEvent(QContextMenuEvent* e)
{
	QMenu menu(this);

	if (editMode == EditMode::characterOverView) {
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
