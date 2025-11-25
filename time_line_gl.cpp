#include "time_line_gl.h"

#include "time_line.h"

#include <QOpenGLShaderProgram>
#include <qmessagebox>
#include <qimage>
#include <qopengltexture>

TimeLineGL::TimeLineGL(QWidget* parent) : QOpenGLWidget(parent)
{
	rect_shader_program = std::make_unique<QOpenGLShaderProgram>(this);
	_timelines.push_back(new Timeline(3.f));
	_timelines.push_back(new Timeline(5.f));
}

TimeLineGL::~TimeLineGL()
{
	for (Timeline* it : _timelines) delete it;

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
	glClearColor(1.f, 1.f, 1.f, 1.f);
	_texture = std::make_unique<QOpenGLTexture>(QImage(":/PiYing/timeline.png").flipped());
	_texture->setWrapMode(QOpenGLTexture::Repeat);

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
	rect_shader_program->setUniformValue("texture1", 0);

	glBindVertexArray(0);

	// global setting

	glActiveTexture(GL_TEXTURE0);
}

void TimeLineGL::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAO);///////////////////////////////////////////////////////

	rect_shader_program->bind();
	_texture->bind();

	for (int i = 0; i < _timelines.size(); i++) {
		Timeline* timeline = _timelines[i];
		rect_shader_program->setUniformValue("selected", i == _current_select);
		rect_shader_program->setUniformValue("lenth", timeline->lenth());
		rect_shader_program->setUniformValue("trans", timeline->get_transform(i));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);////////////////////////////////////////////////////////////
}
