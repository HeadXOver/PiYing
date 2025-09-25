#include "piYingGL.h"

void PiYingGL::fullScreenBackGround()
{
	if (currentSelectedBackGround < 0) return;
	backGrounds[currentSelectedBackGround].transform.reset();
	backGrounds[currentSelectedBackGround].setScale(1 / viewScale.value());
}

void PiYingGL::setViewToStandard()
{
	for (ImageTexture& item : backGrounds) {
		QMatrix4x4 combined = getViewMatrix() * item.getMatrix();
		item.transform.trans = getTrans(combined);
		item.transform.rot = getRot(combined);
		item.transform.scale = getScale(combined);
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
	if (currentSelectedBackGround >= 0) {
		ImageTransform& transform = backGrounds[currentSelectedBackGround].transform;
		transform.trans.setToIdentity();
		transform.rot.setToIdentity();
		transform.scale.setToIdentity();
	}
}

void PiYingGL::bgSetTransform()
{
	if (currentSelectedBackGround >= 0) {
		float transX, transY, Rot, ScaleX, ScaleY;
		ImageTexture& image = backGrounds[currentSelectedBackGround];
		ImageTransform& transform = image.transform;
		float d[5] = { 
			transform.trans(0, 3), 
			transform.trans(1, 3), 
			qAtan2(transform.rot(1, 0), transform.rot(0, 0)) * 180.f / 3.141593f, 
			qSqrt(transform.scale(0,0) * transform.scale(0,0) + transform.scale(1,0) * transform.scale(1,0)), 
			qSqrt(transform.scale(0,1) * transform.scale(0,1) + transform.scale(1,1) * transform.scale(1,1)) 
		};
		QString s[5] = { QString("X位移"), QString("Y位移"), QString("旋转"), QString("X缩放"), QString("Y缩放") };
		if (Ask3DoublesDialog("设置变换", s, d, this).getValues(transX, transY, Rot, ScaleX, ScaleY)) {
			image.setTrans(transX, transY);
			image.setRot(Rot);
			image.setScale(ScaleX, ScaleY);
		}
	}
}

void PiYingGL::againstBg()
{
	if (currentSelectedBackGround >= 0) {
		ImageTransform& transform = backGrounds[currentSelectedBackGround].transform;
		QMatrix4x4 r;
		r.rotate(-qAtan2(transform.rot(1, 0), transform.rot(0, 0)) * 180.f / 3.141593f, 0.0f, 0.0f ,1.0f);
		QPointF toTrans = r.map(QPointF(- transform.trans(0, 3), -transform.trans(1, 3)));
		viewTransX.setValue(toTrans.x());
		viewTransY.setValue(toTrans.y());
		viewScale.setValue(qSqrt(transform.scale(0, 0) * transform.scale(0, 0) + transform.scale(1, 0) * transform.scale(1, 0)));
		viewRotate.setValue(-qAtan2(transform.rot(1, 0), transform.rot(0, 0)) * 180.f / 3.141593f);
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
		backGrounds.removeAt(currentSelectedBackGround);
		currentSelectedBackGround = -1;
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
		backGrounds.clear();
		currentSelectedBackGround = -1;
		update();
	}
}