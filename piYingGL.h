#pragma once

#include <QOpenGLWidget.h>
#include <QOpenGLTexture>
#include <qopenglfunctions_3_3_Core>
#include <QOpenGLShaderProgram.h>

class PiYingGL : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
	Q_OBJECT

public:
	explicit PiYingGL(QWidget* parent = nullptr);
	~PiYingGL();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

public:
	bool lineMode = true;
	bool drawVisible = true;

private:
	QOpenGLShaderProgram shaderProgram;
	QOpenGLTexture* cusTexture = nullptr;
};

