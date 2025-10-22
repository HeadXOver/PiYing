#pragma once

#include <QOpenGLWidget>
#include <qopenglfunctions_3_3_Core>
#include <memory>

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
	std::unique_ptr<QOpenGLShaderProgram> rect_shader_program;
};

