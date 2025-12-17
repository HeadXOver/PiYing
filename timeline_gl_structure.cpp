#include "time_line_gl.h"

#include "time_line.h"
#include "scale_trans.h"

#include <QOpenGLShaderProgram>
#include <qopengltexture>

TimelineGl::TimelineGl(QWidget* parent) : QOpenGLWidget(), _ratio(1.f), _ui_type(UiType::Timeline)
{
	_rect_shader_program = new QOpenGLShaderProgram(this);
	_part_shader_program = new QOpenGLShaderProgram(this);
	_rect_select_program = new QOpenGLShaderProgram(this);

	_scale_trans = new ScaleTrans();
	_last_scale_trans = new ScaleTrans();

	_timelines.push_back(new Timeline(2));
}

TimelineGl::~TimelineGl()
{
	for (Timeline* it : _timelines) delete it;

	makeCurrent();

	////////////////////////////////////////

	glDeleteBuffers(1, &tVBO);
	glDeleteVertexArrays(1, &tVAO);
	glDeleteBuffers(1, &tEBO);

	////////////////////////////////////////

	doneCurrent();
}