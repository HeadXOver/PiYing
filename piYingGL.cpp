#include "piYingGL.h"

#include <QColorDialog>

PiYingGL::PiYingGL(QWidget* parent) : QOpenGLWidget(parent)
{
	setAutoFillBackground(false);

	proj.setToIdentity();
	insProj.setToIdentity();
	viewScale = 1.0f;

	aspect = 16.0f / 9.0f;

	actionAddBackGround = new QAction("添加背景图", this);
	actionFullScreenBackGround = new QAction("背景图全屏", this);
	actionChoseBackGroundColor = new QAction("选择幕布底色", this);
	actionSetViewToStandard = new QAction("将视图设为标准幕布", this);
	actionReturnToStandard = new QAction("返回标准幕布视图", this);

	connect(actionAddBackGround,		SIGNAL(triggered()), this, SLOT(importBackGround()));
	connect(actionFullScreenBackGround, SIGNAL(triggered()), this, SLOT(fullScreenBackGround()));
	connect(actionChoseBackGroundColor, SIGNAL(triggered()), this, SLOT(choseBackGroundColor()));
	connect(actionSetViewToStandard,	SIGNAL(triggered()), this, SLOT(setViewToStandard()));
	connect(actionReturnToStandard,		SIGNAL(triggered()), this, SLOT(returnToStandard()));

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
	glClear(GL_COLOR_BUFFER_BIT);
	for (ImageTexture& it : backGrounds) {
		shaderProgram.bind();
		glActiveTexture(GL_TEXTURE0);
		it.tex->bind();
		shaderProgram.setUniformValue("texture1", 0);
		shaderProgram.setUniformValue("trc", proj * getViewMatrix() * it.trans * it.rot * it.scale * insProj);
		shaderProgram.setUniformValue("selected", it.selected);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(RECTANGLE_VERT), RECTANGLE_VERT, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RECTANGLE_INDECES), RECTANGLE_INDECES, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
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

void PiYingGL::fullScreenBackGround()
{
	if (currentSelectedBackGround < 0) return;
	backGrounds[currentSelectedBackGround].rot.setToIdentity();
	backGrounds[currentSelectedBackGround].scale.setToIdentity();
	backGrounds[currentSelectedBackGround].scale.scale(1 / viewScale);
	backGrounds[currentSelectedBackGround].trans.setToIdentity();
}

void PiYingGL::choseBackGroundColor()
{
	choseBackgroundColor();
}

void PiYingGL::setViewToStandard()
{
	for (ImageTexture& item : backGrounds) {
		item.addScale(viewScale);
		item.addRot(viewRotate);
		item.addTrans(viewTransX, viewTransY);
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

	update();
}

void PiYingGL::importBackground()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, "选择背景图", ".", "Images (*.png *.xpm *.jpg)");

	for (const QString& fileName : fileNames) addBackground(fileName);
}

bool PiYingGL::isInsideSquare(const QPointF& point, float side)
{
	return (point.x() >= -side / 2.f && point.x() <= side / 2.f && point.y() >= -side / 2.f && point.y() <= side / 2.f);
}

MousePos PiYingGL::getMousePosType(const QPointF& point, const ImageTexture& image)
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

void PiYingGL::importBackGround() {
	importBackground();
}