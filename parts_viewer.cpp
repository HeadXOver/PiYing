#include "parts_viewer.h"

#include <QOpenGLShaderProgram>

PartsViewerGl::PartsViewerGl(QWidget* parent)
{
	_shader_program = new QOpenGLShaderProgram(this);
}

PartsViewerGl::~PartsViewerGl()
{
	makeCurrent();

	////////////////////////////////////////

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);

	////////////////////////////////////////

	doneCurrent();
}

void PartsViewerGl::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.1f, 0.1f, 0.2f, 0.4f);

	//////////////initialize background///////////////////////

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	/////////////////////////////////////////////

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(RECTANGLE_VERT), RECTANGLE_VERT, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RECTANGLE_INDECES), RECTANGLE_INDECES, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/square_icon.vert");
	_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/square_icon.frag");
	_shader_program->link();
	_shader_program->setUniformValue("texture1", 0);

	glBindVertexArray(0);

	// global setting

	glActiveTexture(GL_TEXTURE0);
}

void PartsViewerGl::paintGL()
{
}

void PartsViewerGl::wheelEvent(QWheelEvent* ev)
{
}

void PartsViewerGl::mousePressEvent(QMouseEvent* event)
{
}

void PartsViewerGl::mouseReleaseEvent(QMouseEvent* e)
{
}

void PartsViewerGl::mouseMoveEvent(QMouseEvent* event)
{
}
