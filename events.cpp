#include "piYingGL.h"

#include <QMouseEvent>

#include "KeyboardStateWin.h"

void PiYingGL::mousePressEvent(QMouseEvent* event)
{
	LastMousePos = mapToGL(event->position());
	QOpenGLWidget::mousePressEvent(event);

	QPointF posV = QPointF(LastMousePos.x(), LastMousePos.y());
	currentSelectedBackGround = -1;
	LastMousePosType = MousePos::OutSide;
	for (auto& item : backGrounds) item.selected = false;
	if (KeyboardStateWin::isKeyHeld(Qt::Key_B))
		for (int i = backGrounds.size() - 1; i >= 0; i--) {
			ImageTexture& item = backGrounds[i];
			raletiveToRect(posV, item);
			if (isInsideSquare(posV)) {
				LastMousePosType = getMousePosType(posV, item);
				LastImageTransform = { item.trans, item.rot, item.scale };
				item.selected = true;
				currentSelectedBackGround = i;
				break;
			}
		}

	if (currentSelectedBackGround >= 0) {
		posV = backGrounds[currentSelectedBackGround].trans.map(QPointF(0.0f, 0.0f));
		LastSelectCenterToMousePos = QPointF(posV.x() - LastMousePos.x() * aspect, posV.y() - LastMousePos.y());
	}

	currentUpdate();
}

void PiYingGL::mouseMoveEvent(QMouseEvent* event) {
	if (currentSelectedBackGround >= 0) {
		ImageTexture& item = backGrounds[currentSelectedBackGround];
		QPointF mouse = mapToGL(event->position());
		if (event->buttons() == Qt::LeftButton) {
			// move
			if (KeyboardStateWin::isKeyHeld(Qt::Key_B) && LastMousePosType == MousePos::Inside) {
				setCursor(Qt::ClosedHandCursor);
				item.setTrans(mouse.x() * aspect + LastSelectCenterToMousePos.x(), mouse.y() + LastSelectCenterToMousePos.y());
			}
			else if (LastMousePosType == MousePos::Inside) rotationControl(mouse, item);
			else  scaleControl(mouse, item);

			currentUpdate();
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
	}
	else {
		menu.addAction(actionFullScreenBackGround);
	}
	menu.exec(e->globalPos());
	e->accept();
}
