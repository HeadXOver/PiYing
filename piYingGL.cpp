#include "piYingGL.h"
#include "piYing.h"

#include <QColorDialog>

PiYingGL::PiYingGL(PiYing* parent) : QOpenGLWidget(parent), parent(parent)
{
	setAutoFillBackground(false);

	proj.setToIdentity();
	insProj.setToIdentity();
	viewScale.setValue(1.0f);

	aspect = 16.0f / 9.0f;

	actionAddBackGround = new QAction("添加背景图", this);
	actionFullScreenBackGround = new QAction("背景图全屏", this);
	actionChoseBackGroundColor = new QAction("选择幕布底色", this);
	actionSetViewToStandard = new QAction("将视图设为标准幕布", this);
	actionReturnToStandard = new QAction("返回标准幕布视图", this);
	actionDeleteBg = new QAction("删除当前背景图");
	actionDeleteAllBg = new QAction("删除所有背景图");
	actionBgSetTransform = new QAction("设置变换...");

	labelViewScale = new QLabel("1", this);
	labelViewTransX = new QLabel("0", this);
	labelViewTransY = new QLabel("0", this);
	labelViewRot = new QLabel("0", this);

	connect(actionAddBackGround,		SIGNAL(triggered()), this, SLOT(importBackGround()));
	connect(actionFullScreenBackGround, SIGNAL(triggered()), this, SLOT(fullScreenBackGround()));
	connect(actionChoseBackGroundColor, SIGNAL(triggered()), this, SLOT(choseBackGroundColor()));
	connect(actionSetViewToStandard,	SIGNAL(triggered()), this, SLOT(setViewToStandard()));
	connect(actionReturnToStandard,		SIGNAL(triggered()), this, SLOT(returnToStandard()));
	connect(actionDeleteBg,				SIGNAL(triggered()), this, SLOT(deleteBg()));
	connect(actionDeleteAllBg,			SIGNAL(triggered()), this, SLOT(deleteAllBg()));
	connect(actionBgSetTransform,		SIGNAL(triggered()), this, SLOT(bgSetTransform()));
	connect(&viewScale,		&ViewData::valueChanged, this, [&](float v) {labelViewScale->setText(tr("%1  ").arg(v)); });
	connect(&viewTransX,	&ViewData::valueChanged, this, [&](float v) {labelViewTransX->setText(tr("%1  ").arg(v)); });
	connect(&viewTransY,	&ViewData::valueChanged, this, [&](float v) {labelViewTransY->setText(tr("%1  ").arg(v)); });
	connect(&viewRotate,	&ViewData::valueChanged, this, [&](float v) {labelViewRot->setText(tr("%1  ").arg(v)); });

	setMouseTracking(true);

	backGroundColor.setRgbF(0.0f, 0.1f, 0.067f, 1.f);

	framePen.setWidth(6);
}

PiYingGL::~PiYingGL()
{
	makeCurrent();

	////////////////////////////////////////

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);

	////////////////////////////////////////

	doneCurrent();
}

void PiYingGL::paintBackgrounds()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	shaderProgram.bind();
	glActiveTexture(GL_TEXTURE0);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	for (int i = 0; i < backGrounds.size(); i++) {
		ImageTexture& it = backGrounds[i];
		it.tex->bind();

		shaderProgram.setUniformValue("texture1", 0);
		shaderProgram.setUniformValue("trc", getBgShaderMatrix(it.transform));
		if (i == currentSelectedBackGround)shaderProgram.setUniformValue("selected", true);
		else shaderProgram.setUniformValue("selected", false);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PiYingGL::addBackground(const QString& imageName) {
	QImage img;
	if (!img.load(imageName)) {
		QMessageBox::warning(this, "Warning", "Failed to load image: " + imageName);
		return;
	}
	backGrounds.append(img);
	update();
}

void PiYingGL::choseBackgroundColor()
{
	QColor color = QColorDialog::getColor(backGroundColor, this, "选择幕布底色");
	if (!color.isValid()) return;
	backGroundColor = color;
	makeCurrent();
	glClearColor(backGroundColor.redF(), backGroundColor.greenF(), backGroundColor.blueF(), backGroundColor.alphaF());
	glClear(GL_COLOR_BUFFER_BIT);
	doneCurrent();
	update();
}

void PiYingGL::changeRatio(float ratio)
{
	aspect = ratio;
	updateProjMatrix();
	update();
}

void PiYingGL::updateProjMatrix()
{
	if (aspect > 1.0f) {
		proj.setToIdentity();
		proj.ortho(-aspect, aspect, -1, 1, -1, 1);
		insProj.setToIdentity();
		insProj.ortho(-1.0f / aspect, 1.0f / aspect, -1, 1, -1, 1);
	}
	else {
		proj.setToIdentity();
		proj.ortho(-1, 1, -1.0f / aspect, 1.0f / aspect, -1, 1);
		insProj.setToIdentity();
		insProj.ortho(-1, 1, aspect, aspect, -1, 1);
	}
}

void PiYingGL::importBackground()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, "选择背景图", ".", "Images (*.png *.xpm *.jpg)");

	if (fileNames.size() == 1) {
		QImage img;
		if (!img.load(fileNames[0])) {
			QMessageBox::warning(this, "Warning", "Failed to load image: " + fileNames[0]);
			return;
		}
		backGrounds.append(img);

		int ret = QMessageBox::question(
			this,
			tr("提示"),
			tr("是否使用图片比例？"),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::Yes
		);

		if (ret == QMessageBox::Yes) {
			float ratio = img.width() / (float)img.height();
			parent->piYingGLContainer->setRatio(ratio);
			parent->piYingGLContainer->update();
			changeRatio(ratio);
		}

		update();
	}
	else for (const QString& fileName : fileNames) addBackground(fileName);
}

MousePos PiYingGL::getMousePosType(const QPointF& point) const
{
	if (isInsideSquare(point)) {
		if (point.x() < -0.9f) {
			if (point.y() > 0.9f) return MousePos::LeftTop;
			if (point.y() < -0.9f)  return MousePos::LeftBottom;
			return MousePos::LeftEdge;
		}
		if (point.x() > 0.9f) {
			if (point.y() > 0.9f) return MousePos::RightTop;
			if (point.y() < -0.9f)  return MousePos::RightBottom;
			return MousePos::RightEdge;
		}
		if (point.y() > 0.9f) return MousePos::TopEdge;
		if (point.y() < -0.9f) return MousePos::BottomEdge;
		return MousePos::Inside;
	}
	return MousePos::OutSide;
}

Qt::CursorShape PiYingGL::getCursorShape(const MousePos& pos)
{
	switch (pos) {
	case MousePos::LeftTop:
	case MousePos::RightBottom:	//return Qt::SizeFDiagCursor;
	case MousePos::LeftBottom:
	case MousePos::RightTop:	//return Qt::SizeBDiagCursor;
	case MousePos::LeftEdge:
	case MousePos::RightEdge:	//return Qt::SizeHorCursor;
	case MousePos::TopEdge:
	case MousePos::BottomEdge:  if (!KeyboardStateWin::isAltHeld()) return Qt::CursorShape::CrossCursor;
	case MousePos::Inside:		return Qt::OpenHandCursor;
	default:					return Qt::ArrowCursor;
	}
}