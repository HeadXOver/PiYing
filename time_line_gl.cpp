#include "time_line_gl.h"

#include <QOpenGLShaderProgram>

TimeLineGL::TimeLineGL(QWidget* parent) : QOpenGLWidget(parent)
{
	rect_shader_program = std::make_unique<QOpenGLShaderProgram>();
}

TimeLineGL::~TimeLineGL()
{
}

void TimeLineGL::initializeGL()
{
}

void TimeLineGL::paintGL()
{
}
