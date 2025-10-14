﻿#include "piYingGL.h"
#include "piYing.h"
#include "image_transform.h"

PiYingGL::PiYingGL(PiYing* parent) : QOpenGLWidget(parent), parent(parent)
{
	lastImageTransform = new ImageTransform();

	viewScale.setValue(1.0f);
	
	float ratio = 16.f / 9.f;
	proj.ortho(-ratio, ratio, -1, 1, -1, 1);
	insProj.ortho(-1.0f / ratio, 1.0f / ratio, -1, 1, -1, 1);

#pragma region right button menu

	/////////////////////////// right button menu //////////////////////////////////

	QList<QAction*> rightButtonGlobal = {
		new QAction("选择幕布底色", this),
		new QAction("将视图设为标准幕布", this),
		new QAction("返回标准幕布视图", this)
	};

	QAction* actionAddBackGround = new QAction("添加背景图", this);
	QAction* actionAddCharacterTexture = new QAction("添加角色图", this);
	QAction* actionFullScreenBackGround = new QAction("背景图全屏", this);
	QAction* actionDeleteBg = new QAction("删除当前背景图", this);
	QAction* actionDeleteAllBg = new QAction("删除所有背景图", this);
	QAction* actionBgSetTransform = new QAction("设置变换...", this);
	QAction* actionAgainstBg = new QAction("将摄像机对准图片", this);
	QAction* actionReturnbgTransform = new QAction("还原变换", this);

	rightButtonMenu = new  QMenu(this);
	rightButtonMenuChTex = new  QMenu(this);
	rightButtonMenuBg_S = new  QMenu(this);
	rightButtonMenuBg = new  QMenu(this);

	rightButtonMenuChTex->addAction(actionAddCharacterTexture);

	rightButtonMenuBg_S->addAction(actionFullScreenBackGround);
	rightButtonMenuBg_S->addAction(actionDeleteBg);
	rightButtonMenuBg_S->addAction(actionBgSetTransform);
	rightButtonMenuBg_S->addAction(actionAgainstBg);
	rightButtonMenuBg_S->addAction(actionReturnbgTransform);

	rightButtonMenuBg->addAction(actionAddBackGround);
	rightButtonMenuBg->addAction(actionDeleteAllBg);

	addGlobalAction(rightButtonMenu, rightButtonGlobal);
	addGlobalAction(rightButtonMenuChTex, rightButtonGlobal);
	addGlobalAction(rightButtonMenuBg_S, rightButtonGlobal);
	addGlobalAction(rightButtonMenuBg, rightButtonGlobal);

	connect(actionFullScreenBackGround, SIGNAL(triggered()), this, SLOT(fullScreenBackGround()));
	connect(actionDeleteBg, SIGNAL(triggered()), this, SLOT(deleteBg()));
	connect(actionDeleteAllBg, SIGNAL(triggered()), this, SLOT(deleteAllBg()));
	connect(actionAgainstBg, SIGNAL(triggered()), this, SLOT(againstBg()));
	connect(actionBgSetTransform, SIGNAL(triggered()), this, SLOT(bgSetTransform()));
	connect(actionReturnbgTransform, SIGNAL(triggered()), this, SLOT(returnBgTransform()));
	connect(rightButtonGlobal[1], SIGNAL(triggered()), this, SLOT(setViewToStandard()));
	connect(rightButtonGlobal[2], SIGNAL(triggered()), this, SLOT(returnToStandard()));
	connect(rightButtonGlobal[0], &QAction::triggered, this, [this]() {choseBackgroundColor(); });
	connect(actionAddCharacterTexture, &QAction::triggered, this, [this]() {importChatacter(); });
	connect(actionAddBackGround, &QAction::triggered, this, [this]() {importBackground(); });

#pragma endregion

#pragma region label

	////////////////// label ////////////////////////

	QLabel* labelViewScale = new QLabel("1", this);
	QLabel* labelViewTransX = new QLabel("0", this);
	QLabel* labelViewTransY = new QLabel("0", this);
	QLabel* labelViewRot = new QLabel("0", this);

	parent->statusBar()->addPermanentWidget(new QLabel(tr("视图x"), this));
	parent->statusBar()->addPermanentWidget(labelViewTransX);
	parent->statusBar()->addPermanentWidget(new QLabel(tr("视图y"), this));
	parent->statusBar()->addPermanentWidget(labelViewTransY);
	parent->statusBar()->addPermanentWidget(new QLabel(tr("视图旋转"), this));
	parent->statusBar()->addPermanentWidget(labelViewRot);
	parent->statusBar()->addPermanentWidget(new QLabel(tr("视图缩放"), this));
	parent->statusBar()->addPermanentWidget(labelViewScale);

	connect(&viewScale, &ViewData::valueChanged, this, [labelViewScale](float v) {labelViewScale->setText(tr("%1  ").arg(v)); });
	connect(&viewTransX, &ViewData::valueChanged, this, [labelViewTransX](float v) {labelViewTransX->setText(tr("%1  ").arg(v)); });
	connect(&viewTransY, &ViewData::valueChanged, this, [labelViewTransY](float v) {labelViewTransY->setText(tr("%1  ").arg(v)); });
	connect(&viewRotate, &ViewData::valueChanged, this, [labelViewRot](float v) {labelViewRot->setText(tr("%1  ").arg(v)); });

#pragma endregion

	setMouseTracking(true);
}