#include "piYingGL.h"

#include <QMessageBox>
#include <QMouseEvent>

#include "KeyboardStateWin.h"

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

	connect(actionAddBackGround,		SIGNAL(triggered()), this, SLOT(importBackGround()));
	connect(actionFullScreenBackGround, SIGNAL(triggered()), this, SLOT(fullScreenBackGround()));

	setMouseTracking(true);
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

	glClearColor(0.0f, 0.1f, 0.067f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PiYingGL::resizeGL(int w, int h){

}

void PiYingGL::paintGL(){
	paintBackgrounds();
}

void PiYingGL::mousePressEvent(QMouseEvent* event)
{
	QOpenGLWidget::mousePressEvent(event);

	QPointF mouse = mapToGL(event->position());
	QPointF posV = QPointF(mouse.x(), mouse.y());
	currentSelectedBackGround = -1;
	for (auto& item : backGrounds) item.selected = false;
	if (KeyboardStateWin::isKeyHeld(Qt::Key_B))
		for (int i = backGrounds.size() - 1; i >= 0; i--) {
			ImageTexture& item = backGrounds[i];
			raletiveToRect(posV, item);
			if (isInsideSquare(posV)) {
				item.selected = true;
				currentSelectedBackGround = i;
				break;
			}
		}

	if (currentSelectedBackGround >= 0) {
		posV = backGrounds[currentSelectedBackGround].trans.map(QPointF(0.0f, 0.0f));
		LastSelectMousePos = QPointF(posV.x() - mouse.x(), posV.y() - mouse.y());
	}

	currentUpdate();
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

void PiYingGL::raletiveToRect(QPointF& point, const ImageTexture& image)
{
	QMatrix4x4 mViewScale;
	mViewScale.scale(1 / viewScale);	
	point = (proj * mViewScale * image.scale.inverted() * image.rot.inverted() * image.trans.inverted() * insProj).map(point);
}

void PiYingGL::raletiveToRect(QVector4D& vec4, const ImageTexture& image)
{
	QMatrix4x4 mViewScale;
	mViewScale.scale(1 / viewScale);
	vec4 = proj * mViewScale * image.scale.inverted() * image.rot.inverted() * image.trans.inverted() * insProj * vec4;
}

bool PiYingGL::isInsideSquare(const QPointF& point, float side)
{
	return (point.x() >= -side / 2.f && point.x() <= side / 2.f && point.y() >= -side / 2.f && point.y() <= side / 2.f);
}

QPointF PiYingGL::mapToGL(const QPointF& point)
{
	return QPointF((point.x() / float(width())) * 2.0f - 1.0f, 1.0f - (point.y() / float(height())) * 2.0f);
}

void PiYingGL::mouseMoveEvent(QMouseEvent* event) {
	if (currentSelectedBackGround >= 0) {
		QPointF mouse = mapToGL(event->position());
		QPointF mouseRaletive = mouse;
		raletiveToRect(mouseRaletive, backGrounds[currentSelectedBackGround]);
		if (event->buttons() == Qt::LeftButton) {
			if (isInsideSquare(mouseRaletive, 1.8f)) {
				QMatrix4x4 toTrans;
				toTrans.translate(mouse.x() + LastSelectMousePos.x(), mouse.y() + LastSelectMousePos.y());
				backGrounds[currentSelectedBackGround].trans = toTrans;

				currentUpdate();
			}
		}
		else {
			if (isInsideSquare(mouseRaletive)) {
				if (mouseRaletive.x() < -0.9f) {
					if (mouseRaletive.y() > 0.9f) {
						setCursor(Qt::SizeFDiagCursor);
					}
					else if (mouseRaletive.y() < -0.9f) {
						setCursor(Qt::SizeBDiagCursor);
					}
					else {
						setCursor(Qt::SizeHorCursor);
					}
					if (mouseRaletive.y() > 0.9f) {
						if (mouseRaletive.x() < -0.9f) {
							setCursor(Qt::SizeFDiagCursor);
						}
						else if (mouseRaletive.x() > 0.9f) {
							setCursor(Qt::SizeBDiagCursor);
						}
						else {
							setCursor(Qt::SizeVerCursor);
						}
					}
				}
				else if(mouseRaletive.x() > 0.9f) {
					if (mouseRaletive.y() > 0.9f) {
						setCursor(Qt::SizeBDiagCursor);
					}
					else if (mouseRaletive.y() < -0.9f) {
						setCursor(Qt::SizeFDiagCursor);
					}
					else {
						setCursor(Qt::SizeHorCursor);
					}
					if (mouseRaletive.y() > 0.9f) {
						if (mouseRaletive.x() < -0.9f) {
							setCursor(Qt::SizeFDiagCursor);
						}
						else if (mouseRaletive.x() > 0.9f) {
							setCursor(Qt::SizeBDiagCursor);
						}
						else {
							setCursor(Qt::SizeVerCursor);
						}
					}
				}
				else if(mouseRaletive.y() > 0.9f || mouseRaletive.y() < -0.9f) {
					setCursor(Qt::SizeVerCursor);
				}
				else {
					setCursor(Qt::OpenHandCursor);
				}
			}
			else {
				setCursor(Qt::ArrowCursor);
			}
			currentUpdate();
		}
	}
	else {
		setCursor(Qt::ArrowCursor);
	}
}

void PiYingGL::wheelEvent(QWheelEvent* ev){
	QPoint numSteps = ev->angleDelta() / 120.f;

	if (!numSteps.isNull()) {
		int delta = numSteps.y();
		float scaleFactor = 1.0f + delta * 0.1f;
		if (scaleFactor < 0.1f) scaleFactor = 0.1f;
		viewScale *= scaleFactor;

		currentUpdate();
	}

	ev->accept();   // self event
}

void PiYingGL::contextMenuEvent(QContextMenuEvent* e)
{
	// QMessageBox::information(this, "Info", "Right click at (" + QString::number(e->pos().x()) + ", " + QString::number(e->pos().y()) + ")");

	QMenu menu(this);
	if (currentSelectedBackGround < 0) {
		menu.addAction(actionAddBackGround);
	}
	else {
		menu.addAction(actionFullScreenBackGround);
	}
	menu.exec(e->globalPos());
	e->accept();
}


void PiYingGL::importBackGround() {
	importBackground();
}