#include "piYingGL.h"

void PiYingGL::fullScreenBackGround()
{
	if (currentSelectedBackGround < 0) return;
	backGrounds[currentSelectedBackGround].transform.reset();
	backGrounds[currentSelectedBackGround].setScale(1 / viewScale);
}

void PiYingGL::choseBackGroundColor()
{
	choseBackgroundColor();
}

void PiYingGL::setViewToStandard()
{
	for (ImageTexture& item : backGrounds) {
		QMatrix4x4 combined = getViewMatrix() * item.getMatrix();
		item.transform.trans = getTrans(combined);
		item.transform.rot = getRot(combined);
		item.transform.scale = getScale(combined);
	}

	viewRotate = 0.f;
	viewScale = 1.f;
	viewTransX = 0.f;
	viewTransY = 0.f;
}

void PiYingGL::returnToStandard()
{
	viewScale = 1.0f;
	viewRotate = 0.f;
	viewTransX = 0.f;
	viewTransY = 0.f;
	update();
}

void PiYingGL::deleteBg()
{
	backGrounds.removeAt(currentSelectedBackGround);
	currentSelectedBackGround = -1;
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
	}
}

void PiYingGL::importBackGround() {
	importBackground();
}