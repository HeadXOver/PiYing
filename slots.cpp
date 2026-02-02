#include "piYingGL.h"
#include "PiYing.h"
#include "cus_func_transform.h"
#include "image_transform.h"
#include "image_texture.h"
#include "AskTransformDialog.h"
#include "cus_ask.h"

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
	for (auto item : backGrounds) {
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
	update_trc();
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
		const ImageTransform& transform = image->transform();
		float d[5] = {
			transform.get_trans_x(),
			transform.get_trans_y(),
			transform.get_rot_degree(),
			transform.get_scale_x(),
			transform.get_scale_y()
		};
		if (AskBgTransformDialog("设置变换", d, this).getValues(transX, transY, Rot, ScaleX, ScaleY)) {
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
		const ImageTransform& transform = backGrounds[cur]->transform();
		QMatrix4x4 r;
		r.rotate(-transform.get_rot_degree(), 0.0f, 0.0f, 1.0f);
		QPointF toTrans = r.map(QPointF(-transform.get_trans_x(), -transform.get_trans_y()));
		viewTransX.setValue(toTrans.x() / transform.get_scale_x());
		viewTransY.setValue(toTrans.y() / transform.get_scale_x());
		viewScale.setValue(1 / transform.get_scale_x());
		viewRotate.setValue(-transform.get_rot_degree());
	}

	update();
}

void PiYingGL::deleteBg()
{
	if (!CUS_YES_OR_NO("删除背景图？")) return;

	delete backGrounds[getCurrentBgRow()];
	backGrounds.removeAt(getCurrentBgRow());
	PiYing::getInstance().delete_current_bg_item();

	update();
}

void PiYingGL::deleteAllBg()
{
	if (!CUS_YES_OR_NO("删除全部背景图？")) return;

	for (ImageTexture* item : backGrounds) {
		delete item;
	}

	backGrounds.clear();

	PiYing::getInstance().delete_all_bg_item();

	assert(backGrounds.empty());

	update();
}