#pragma once

#include <QOpenGLWidget>
#include <qopenglfunctions_3_3_Core>
#include <memory>
#include <vector>

#include "static_rect_vert.h"

class QOpenGLShaderProgram;
class QOpenGLTexture;
class Timeline;

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
	unsigned int VAO = 0, VBO = 0, EBO = 0;

	int _current_select{ -1 };

	std::unique_ptr<QOpenGLShaderProgram> rect_shader_program;

	std::unique_ptr<QOpenGLTexture> _texture;

	std::vector<Timeline*> _timelines;
};

