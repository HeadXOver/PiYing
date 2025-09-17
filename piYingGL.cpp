#include "piYingGL.h"

unsigned int VAO = 0, VBO = 0, EBO = 0;

PiYingGL::PiYingGL(QWidget* parent) : QOpenGLWidget(parent)
{
	setAutoFillBackground(false);
}

PiYingGL::~PiYingGL()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);

	if (cusTexture)
	{
		delete cusTexture;
		cusTexture = nullptr;
	}
}

void PiYingGL::initializeGL()
{
	initializeOpenGLFunctions();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	/////////////////////////////////////////////

	shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/shapes.vert");
	shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/shapes.frag");
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
	glClear(GL_COLOR_BUFFER_BIT);
}
