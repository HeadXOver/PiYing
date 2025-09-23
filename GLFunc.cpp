#include "piYingGL.h"

void PiYingGL::initializeGL()
{
	initializeOpenGLFunctions();

	aspect = width() / float(height());
	updateProjMatrix();

	//////////////initialize background///////////////////////

	glGenVertexArrays(1, &bgVAO);
	glGenBuffers(1, &bgVBO);
	glGenBuffers(1, &bgEBO);

	glBindVertexArray(bgVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);

	/////////////////////////////////////////////

	glBufferData(GL_ARRAY_BUFFER, sizeof(RECTANGLE_VERT), RECTANGLE_VERT, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RECTANGLE_INDECES), RECTANGLE_INDECES, GL_STATIC_DRAW);

	bgShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/bgshapes.vert");
	bgShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/bgshapes.frag");
	bgShaderProgram.link();

	/////////////////////////////////////////////

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	////////////////initialize character texture///////////////////////

	glGenVertexArrays(1, &chVAO);
	glGenBuffers(1, &chVBO);
	glGenBuffers(1, &chEBO);

	glBindVertexArray(chVAO);
	glBindBuffer(GL_ARRAY_BUFFER, chVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chEBO);

	/////////////////////////////////////////////

	chShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/bgshapes.vert");
	chShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/bgshapes.frag");
	chShaderProgram.link();

	/////////////////////////////////////////////

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	////////////////end initialize//////////////////////////

	glClearColor(backGroundColor.redF(), backGroundColor.greenF(), backGroundColor.blueF(), backGroundColor.alphaF());
	glClear(GL_COLOR_BUFFER_BIT);
}

void PiYingGL::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);
	if(editMode != EditMode::characterOverView) paintBackgrounds();
	else {
		paintCharactersOverView();
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