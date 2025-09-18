#include "piYingGL.h"

#include <QMessageBox>
#include <QMouseEvent>

unsigned int VAO = 0, VBO = 0, EBO = 0;

PiYingGL::PiYingGL(QWidget* parent) : QOpenGLWidget(parent)
{
	setAutoFillBackground(false);

	proj.setToIdentity();
	insProj.setToIdentity();

	aspect = 16.0f / 9.0f;
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
		shaderProgram.setUniformValue("trc", proj * it.trans * it.rot * it.scale * insProj);
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

	glClearColor(0.2f, 0.1f, 0.067f, 1.f);
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

	QPointF mouse = event->pos();
	float x = (mouse.x() / float(width())) * 2.0f - 1.0f;
	float y = 1.0f - (mouse.y() / float(height())) * 2.0f;
	QVector4D posV;
	currentSelectedBackGround = -1;
	for (auto& item : backGrounds) item.selected = false;
	for (int i = backGrounds.size() - 1; i >= 0; i--){
		ImageTexture& item = backGrounds[i];
		posV = proj * item.scale.inverted() * item.rot.inverted() * item.trans.inverted() * insProj * QVector4D(x, y, 0.0f, 1.0f);
		if (posV.x() >= -1.0f && posV.x() <= 1.0f && posV.y() >= -1.0f && posV.y() <= 1.0f) {
			item.selected = true;
			currentSelectedBackGround = i;
			break;
		}
	}

	if(currentSelectedBackGround >= 0) posV = backGrounds[currentSelectedBackGround].trans * QVector4D(0.0f, 0.0f, 0.0f, 1.0f);
	LastSelectMousePos = QPointF(posV.x() - x, posV.y() - y);

	update();
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

void PiYingGL::mouseMoveEvent(QMouseEvent* event) {
	if (event->buttons() && Qt::LeftButton && currentSelectedBackGround >= 0) {
		QPointF mouse = event->pos();
		float x = (mouse.x() / float(width())) * 2.0f - 1.0f;
		float y = 1.0f - (mouse.y() / float(height())) * 2.0f;
		QMatrix4x4 toTrans;
		toTrans.translate(x + LastSelectMousePos.x(), y + LastSelectMousePos.y());
		backGrounds[currentSelectedBackGround].trans = toTrans;
	}

	makeCurrent();
	paintBackgrounds();
	update();
}