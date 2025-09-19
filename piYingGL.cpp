#include "piYingGL.h"

#include <QMessageBox>
#include <QColorDialog>

unsigned int VAO = 0, VBO = 0, EBO = 0;

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

	connect(actionAddBackGround,		SIGNAL(triggered()), this, SLOT(importBackGround()));
	connect(actionFullScreenBackGround, SIGNAL(triggered()), this, SLOT(fullScreenBackGround()));
	connect(actionChoseBackGroundColor, SIGNAL(triggered()), this, SLOT(choseBackGroundColor()));

	setMouseTracking(true);

	backGroundColor.setRgbF(0.0f, 0.1f, 0.067f, 1.f);
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
		QMatrix4x4 mViewScale;
		mViewScale.scale(viewScale);
		shaderProgram.setUniformValue("texture1", 0);
		shaderProgram.setUniformValue("trc", proj * it.trans * it.rot * it.scale * mViewScale * insProj);
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

void PiYingGL::initializeGL()
{
	aspect = width() / float(height());
	initializeOpenGLFunctions();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	/////////////////////////////////////////////

	shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/bgshapes.vert");
	shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/bgshapes.frag");
	shaderProgram.link();
	shaderProgram.bind();

	/////////////////////////////////////////////

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glClearColor(backGroundColor.redF(), backGroundColor.greenF(), backGroundColor.blueF(), backGroundColor.alphaF());
	glClear(GL_COLOR_BUFFER_BIT);
}

void PiYingGL::resizeGL(int w, int h){

}

void PiYingGL::paintGL(){
	paintBackgrounds();
}

void PiYingGL::addBackground(QString& imageName){
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
	QString fileName = QFileDialog::getOpenFileName(this, "选择背景图", ".", "Images (*.png *.xpm *.jpg)");
	if (fileName.isEmpty()) {
		return;
	}
	addBackground(fileName);
}

void PiYingGL::currentUpdate()
{
	makeCurrent();
	paintBackgrounds();
	update();
	doneCurrent();
}

void PiYingGL::rotationControl(const QPointF& mouse, ImageTexture& image)
{
	QPointF center = LastImageTransform.trans.map(QPointF(0.f, 0.f));
	QPointF vec1 = LastMousePos - center;
	QPointF vec2 = mouse - center;
	float r = (atan2f(vec2.y(), vec2.x()) - atan2f(vec1.y(), vec1.x())) * 180.f / 3.1415926f;

	image.setRot(r);
	image.rot *= LastImageTransform.rot;
}

void PiYingGL::scaleControl(const QPointF& mouse, ImageTexture& image)
{
	QPointF mouseRaletiveStatic = getRaletiveToRect(QPointF(mouse.x(), mouse.y()), LastImageTransform);
	QPointF LastMouseRaletive = getRaletiveToRect(QPointF(LastMousePos.x(), LastMousePos.y()), LastImageTransform);
	QPointF toMoveRaletive = getRaletiveToRectWithoutTrans((mouse - LastMousePos), LastImageTransform);

	image.trans = LastImageTransform.trans;

	if (LastMousePosType == MousePos::BottomEdge) {
		image.setScale(1.f, (1.0f - mouseRaletiveStatic.y()) / (1.0f - LastMouseRaletive.y()));
		toMoveRaletive = getRaletiveToGlobalWithoutTrans(QPointF(0.f, toMoveRaletive.y() / 2.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f - LastMouseRaletive.y()) * 2.f, toMoveRaletive.y() / (1.0f - LastMouseRaletive.y()) * 2.f);
	}
	else if(LastMousePosType == MousePos::TopEdge) {
		image.setScale(1.f, (1.0f + mouseRaletiveStatic.y()) / (1.0f + LastMouseRaletive.y()));
		toMoveRaletive = getRaletiveToGlobalWithoutTrans(QPointF(0.f, toMoveRaletive.y() / 2.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f + LastMouseRaletive.y()) * 2.f, toMoveRaletive.y() / (1.0f + LastMouseRaletive.y()) * 2.f);
	}
	else if (LastMousePosType == MousePos::LeftEdge) {
		image.setScale((1.0f - mouseRaletiveStatic.x()) / (1.0f - LastMouseRaletive.x()), 1.f);
		toMoveRaletive = getRaletiveToGlobalWithoutTrans(QPointF(toMoveRaletive.x() / 2.f, 0.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f - LastMouseRaletive.x()) * 2.f, toMoveRaletive.y() / (1.0f - LastMouseRaletive.x()) * 2.f);
	}
	else if (LastMousePosType == MousePos::RightEdge) {
		image.setScale((1.0f + mouseRaletiveStatic.x()) / (1.0f + LastMouseRaletive.x()), 1.f);
		toMoveRaletive =  getRaletiveToGlobalWithoutTrans(QPointF(toMoveRaletive.x() / 2.f, 0.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f + LastMouseRaletive.x()) * 2.f, toMoveRaletive.y() / (1.0f + LastMouseRaletive.x()) * 2.f);
	}
	else if (LastMousePosType == MousePos::LeftTop) {
		image.setScale((1.0f - mouseRaletiveStatic.x()) / (1.0f - LastMouseRaletive.x()), (1.0f + mouseRaletiveStatic.y()) / (1.0f + LastMouseRaletive.y()));
		toMoveRaletive = getRaletiveToGlobalWithoutTrans(QPointF(toMoveRaletive.x() / 2.f, toMoveRaletive.y() / 2.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f - LastMouseRaletive.x()) * 2.f, toMoveRaletive.y() / (1.0f + LastMouseRaletive.y()) * 2.f);
	}
	else if (LastMousePosType == MousePos::LeftBottom) {
		image.setScale((1.0f - mouseRaletiveStatic.x()) / (1.0f - LastMouseRaletive.x()), (1.0f - mouseRaletiveStatic.y()) / (1.0f - LastMouseRaletive.y()));
		toMoveRaletive = getRaletiveToGlobalWithoutTrans(QPointF(toMoveRaletive.x() / 2.f, toMoveRaletive.y() / 2.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f - LastMouseRaletive.x()) * 2.f, toMoveRaletive.y() / (1.0f - LastMouseRaletive.y()) * 2.f);
	}
	else if (LastMousePosType == MousePos::RightTop) {
		image.setScale((1.0f + mouseRaletiveStatic.x()) / (1.0f + LastMouseRaletive.x()), (1.0f + mouseRaletiveStatic.y()) / (1.0f + LastMouseRaletive.y()));
		toMoveRaletive = getRaletiveToGlobalWithoutTrans(QPointF(toMoveRaletive.x() / 2.f, toMoveRaletive.y() / 2.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f + LastMouseRaletive.x()) * 2.f, toMoveRaletive.y() / (1.0f + LastMouseRaletive.y()) * 2.f);
	}
	else if (LastMousePosType == MousePos::RightBottom) {
		image.setScale((1.0f + mouseRaletiveStatic.x()) / (1.0f + LastMouseRaletive.x()), (1.0f - mouseRaletiveStatic.y()) / (1.0f - LastMouseRaletive.y()));
		toMoveRaletive = getRaletiveToGlobalWithoutTrans(QPointF(toMoveRaletive.x() / 2.f, toMoveRaletive.y() / 2.f), LastImageTransform);
		image.trans.translate(toMoveRaletive.x() * aspect / (1.0f + LastMouseRaletive.x()) * 2.f, toMoveRaletive.y() / (1.0f - LastMouseRaletive.y()) * 2.f);
	}


	image.scale *= LastImageTransform.scale;
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
	case MousePos::Inside:		return Qt::OpenHandCursor;
	case MousePos::LeftTop:
	case MousePos::RightBottom:	//return Qt::SizeFDiagCursor;
	case MousePos::LeftBottom:
	case MousePos::RightTop:	//return Qt::SizeBDiagCursor;
	case MousePos::LeftEdge:
	case MousePos::RightEdge:	//return Qt::SizeHorCursor;
	case MousePos::TopEdge:
	case MousePos::BottomEdge:	return Qt::CursorShape::SizeAllCursor;
	default:					return Qt::ArrowCursor;
	}
}

void PiYingGL::importBackGround() {
	importBackground();
}