#include "piYingGL.h"
#include "PiYing.h"
#include "CusFunc.h"
#include "image_transform.h"
#include "image_texture.h"

#include <qmessagebox>

void PiYingGL::fullScreenBackGround()
{
	int cur = getCurrentBgRow();
	if (cur < 0) return;
	backGrounds[cur]->resetTransform();
	backGrounds[cur]->setScale(1 / viewScale.value());
}

void PiYingGL::setViewToStandard()
{
	for (ImageTexture* item : backGrounds) {
		QMatrix4x4 combined = getViewMatrix() * item->getMatrix();
		item->setTrans(getTrans(combined));
		item->setRot(getRot(combined));
		item->setScale(getScale(combined));
	}

	returnToStandard();
}

void PiYingGL::returnToStandard()
{
	viewScale.setValue(1.0f);
	viewRotate.setValue(0.f);
	viewTransX.setValue(0.f);
	viewTransY.setValue(0.f);
	update();
}

void PiYingGL::returnBgTransform()
{
	int cur = getCurrentBgRow();
	if (cur >= 0) {
		backGrounds[cur]->resetTransform();
	}
}

void PiYingGL::bgSetTransform()
{
	int cur = getCurrentBgRow();
	if (cur >= 0) {
		float transX, transY, Rot, ScaleX, ScaleY;
		ImageTexture* image = backGrounds[cur];
		ImageTransform* transform = image->transform();
		float d[5] = { 
			transform->trans(0, 3), 
			transform->trans(1, 3),
			qAtan2(transform->rot(1, 0), transform->rot(0, 0)) * 180.f / 3.141593f,
			qSqrt(transform->scale(0,0) * transform->scale(0,0) + transform->scale(1,0) * transform->scale(1,0)),
			qSqrt(transform->scale(0,1) * transform->scale(0,1) + transform->scale(1,1) * transform->scale(1,1))
		};
		QString s[5] = { QString("X位移"), QString("Y位移"), QString("旋转"), QString("X缩放"), QString("Y缩放") };
		if (Ask3DoublesDialog("设置变换", s, d, this).getValues(transX, transY, Rot, ScaleX, ScaleY)) {
			image->setTrans(transX, transY);
			image->setRot(Rot);
			image->setScale(ScaleX, ScaleY);
		}
	}
}

void PiYingGL::againstBg()
{
	int cur = getCurrentBgRow();
	if (cur >= 0) {
		ImageTransform* transform = backGrounds[cur]->transform();
		QMatrix4x4 r;
		r.rotate(-qAtan2(transform->rot(1, 0), transform->rot(0, 0)) * 180.f / 3.141593f, 0.0f, 0.0f ,1.0f);
		QPointF toTrans = r.map(QPointF(- transform->trans(0, 3), -transform->trans(1, 3)));
		viewTransX.setValue(toTrans.x());
		viewTransY.setValue(toTrans.y());
		viewScale.setValue(qSqrt(transform->scale(0, 0) * transform->scale(0, 0) + transform->scale(1, 0) * transform->scale(1, 0)));
		viewRotate.setValue(-qAtan2(transform->rot(1, 0), transform->rot(0, 0)) * 180.f / 3.141593f);
	}
}

void PiYingGL::deleteBg()
{
	int ret = QMessageBox::question(
		this,
		tr("提示"),
		tr("删除背景图？"),
		QMessageBox::Yes | QMessageBox::No,
		QMessageBox::No
	);

	if (ret == QMessageBox::Yes) {
		delete backGrounds[getCurrentBgRow()];
		backGrounds.removeAt(getCurrentBgRow());
		QListWidgetItem* item = parent->bgImageList->takeItem(getCurrentBgRow());
		delete item;
		update();
	}
}

void PiYingGL::deleteAllBg()
{
	int ret = QMessageBox::question(
		this,
		tr("提示"),
		tr("删除全部背景图？"),
		QMessageBox::Yes | QMessageBox::No,
		QMessageBox::No
	);

	if (ret == QMessageBox::Yes) {
		for (ImageTexture* item : backGrounds) {
			delete item;
		}
		backGrounds.clear();
		parent->bgImageList->clear();
		update();
	}
}