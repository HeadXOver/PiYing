#include "time_line_gl.h"

#include <QOpenGLShaderProgram>
#include <qmessagebox>

TimeLineGL::TimeLineGL(QWidget* parent) : QOpenGLWidget(parent)
{
	rect_shader_program = std::make_unique<QOpenGLShaderProgram>(this);
}

TimeLineGL::~TimeLineGL()
{
	makeCurrent();

	////////////////////////////////////////

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);

	////////////////////////////////////////

	doneCurrent();
}

void TimeLineGL::initializeGL()
{
	initializeOpenGLFunctions();

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

	rect_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/PiYing/timeline_rect_shape.vert");
	rect_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/PiYing/timeline_rect_shape.frag");

	rect_shader_program->link();
	rect_shader_program->bind();

	glBindVertexArray(0);

	// global setting

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TimeLineGL::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAO);///////////////////////////////////////////////////////

	rect_shader_program->bind();
	rect_shader_program->setUniformValue("selected", false);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);////////////////////////////////////////////////////////////
}
