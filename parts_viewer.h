#pragma once

#include <QOpenGLWidget>
#include <qopenglfunctions_3_3_Core>

#include "static_rect_vert.h"

class QOpenGLShaderProgram;

class PartsViewerGl final: public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
	Q_OBJECT

public:
	PartsViewerGl(QWidget* parent);
	~PartsViewerGl();

protected:
	void initializeGL() override;
	void paintGL() override;

	void wheelEvent(QWheelEvent* ev) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	unsigned int VAO = 0, VBO = 0, EBO = 0;

	QOpenGLShaderProgram* _shader_program;
};

