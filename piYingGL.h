#pragma once

#include <QOpenGLWidget.h>
#include <qopenglfunctions_3_3_Core>
#include <QOpenGLShaderProgram.h>

#include "Element.h"

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
	void addBackground(QString& imageName);

public:
	bool lineMode = true;
	bool drawVisible = true;

private:
	QOpenGLShaderProgram shaderProgram;

	// imageTextures
	QList<ImageTexture> backGrounds;
	QList<ImageTexture> characterTextures;
};

