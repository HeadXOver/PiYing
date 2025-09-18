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

private:
	void paintBackgrounds();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

public:
	void addBackground(QString& imageName);
	void changeRatio(float ratio);

public:
	bool lineMode = true;
	bool drawVisible = true;

private:
	QOpenGLShaderProgram shaderProgram;

	// imageTextures
	QList<ImageTexture> backGrounds;
	QList<ImageTexture> characterTextures;

	int currentSelectedBackGround = -1;

	float aspect;

	QMatrix4x4 proj;
	QMatrix4x4 insProj;

	QPointF LastSelectMousePos;
};

