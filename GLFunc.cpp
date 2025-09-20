#include "piYingGL.h"

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

void PiYingGL::paintGL() {
	paintBackgrounds();
	if (editMode == EditMode::BackGround) {
		QPainter p(this);
		p.setPen(framePen);
		p.drawRect(rect());
	}
}

void PiYingGL::resizeGL(int w, int h) {

}

void PiYingGL::currentUpdate()
{
	makeCurrent();
	paintBackgrounds();
	update();
	doneCurrent();
}