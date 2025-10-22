#pragma once

#include <QOpenGLWidget>
#include <qopenglfunctions_3_3_Core>
#include <memory>

#include "static_rect_vert.h"

class QOpenGLShaderProgram;

class TimeLineGL : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
	Q_OBJECT

public:
	TimeLineGL(QWidget* parent);
	~TimeLineGL();

protected:
	void initializeGL() override;
	void paintGL() override;

private:
	unsigned int VAO, VBO, EBO;

	std::unique_ptr<QOpenGLShaderProgram> rect_shader_program;
};

