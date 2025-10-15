﻿#include "piYingGL.h"
#include "PiYing.h"
#include "cus_func_transform.h"
#include "image_transform.h"
#include "image_texture.h"
#include "AskTransformDialog.h"

#include <qmessagebox>
#include <qlistwidget>

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

	update();
}

void PiYingGL::bgSetTransform()
{
	int cur = getCurrentBgRow();
	if (cur >= 0) {
		float transX, transY, Rot, ScaleX, ScaleY;
		ImageTexture* image = backGrounds[cur];
		ImageTransform* transform = image->transform();
		float d[5] = {
			transform->get_trans_x(),
			transform->get_scale_y(),
			transform->get_rot_degree(),
			transform->get_scale_x(),
			transform->get_scale_y()
		};
		QString s[5] = { QString("X位移"), QString("Y位移"), QString("旋转"), QString("X缩放"), QString("Y缩放") };
		if (Ask3DoublesDialog("设置变换", s, d, this).getValues(transX, transY, Rot, ScaleX, ScaleY)) {
			image->setTrans(transX, transY);
			image->setRot(Rot);
			image->setScale(ScaleX, ScaleY);
		}
	}

	update();
}

void PiYingGL::againstBg()
{
	int cur = getCurrentBgRow();
	if (cur >= 0) {
		ImageTransform* transform = backGrounds[cur]->transform();
		QMatrix4x4 r;
		r.rotate(-transform->get_rot_degree(), 0.0f, 0.0f, 1.0f);
		QPointF toTrans = r.map(QPointF(-transform->get_trans_x(), -transform->get_trans_y()));
		viewTransX.setValue(toTrans.x() / transform->get_scale_x());
		viewTransY.setValue(toTrans.y() / transform->get_scale_x());
		viewScale.setValue(1 / transform->get_scale_x());
		viewRotate.setValue(-transform->get_rot_degree());
	}

	update();
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