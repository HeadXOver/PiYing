#include "piYingGL.h"
#include "PiYing.h"
#include "image_transform.h"
#include "image_texture.h"
#include "KeyboardStateWin.h"
#include "ch_element_tool.h"

#include <qmouseevent>
#include <qmenu>

void PiYingGL::mousePressEvent(QMouseEvent* event)
{
	lastMousePos = mapToGL(event->position());
	if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
		if (editMode == EditMode::BackGround) {
			lastMousePosType = MousePos::OutSide;
			int cur = getCurrentBgRow();

			if(cur < 0) return;

			ImageTexture* item = backGrounds[cur];
			QPointF posV = getRaletiveToRect(lastMousePos, item->transform());
			if (isInsideSquare(posV)) {
				lastMousePosType = getMousePosType(posV);
				*lastImageTransform = *item->transform();
			}
			update();
			return;
		}

		if ((editMode == EditMode::characterTexture || editMode == EditMode::characterSkeleton || editMode == EditMode::controlSlide)
			&& event->button() == Qt::LeftButton) {
			if (ch_element_tool_) ch_element_tool_->click(event->position());
			update();
			return;
		}
		return;
	}

	if (event->button() == Qt::MiddleButton) {
		lastMiddleButtonPos = mapToGL(event->position());
		lastViewTransX = viewTransX.value();
		lastViewTransY = viewTransY.value();
		lastViewRotate = viewRotate.value(); 
		return;
	}
}

void PiYingGL::mouseReleaseEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton && ch_element_tool_) ch_element_tool_->release(e->position());

	update();
}

void PiYingGL::mouseMoveEvent(QMouseEvent* event) {
	QPointF mouse = mapToGL(event->position());
	if (event->buttons() == Qt::LeftButton) {
		if (lastMousePosType != MousePos::OutSide) {
			ImageTexture* item = backGrounds[ref_PiYing.getCurrentBgRow()];
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
		else if (editMode == EditMode::characterTexture || editMode == EditMode::characterSkeleton || editMode == EditMode::controlSlide) {
			if (ch_element_tool_) {
				ch_element_tool_->move(event->position());
				update();
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
		if (editMode != EditMode::BackGround) return;

		int cur = ref_PiYing.getCurrentBgRow();
		if (cur >= 0) setCursor(getCursorShape(getMousePosType(getRaletiveToRect(mouse, backGrounds[cur]->transform()))));
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
