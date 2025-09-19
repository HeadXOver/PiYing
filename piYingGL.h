#pragma once

#include <QOpenGLWidget.h>
#include <qopenglfunctions_3_3_Core>
#include <QOpenGLShaderProgram.h>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QMenu>
#include <QMenuBar>

#include "Element.h"

enum class MousePos {
	Inside,
	LeftTop,
	LeftBottom,
	RightTop,
	RightBottom,
	LeftEdge,
	RightEdge,
	TopEdge,
	BottomEdge,
	OutSide
};

class PiYingGL : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
	Q_OBJECT

public:
	explicit PiYingGL(QWidget* parent = nullptr);
	~PiYingGL();

private:
	void paintBackgrounds();

private slots:
	void importBackGround();
	void fullScreenBackGround();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* ev) override;
	void contextMenuEvent(QContextMenuEvent* e) override;

public:
	void addBackground(QString& imageName);
	void changeRatio(float ratio);
	void importBackground();
	void currentUpdate();
	void rotationControl(const QPointF& mouse, ImageTexture& image);
	void scaleControl(const QPointF& mouse, ImageTexture& image);

	void raletiveToRect(QVector4D& vec4, const ImageTexture& image);
	void raletiveToRect(QPointF& point, const ImageTexture& image);
	void raletiveToRect(QPointF& point, const ImageTransform& transform);
	void raletiveToRectWithoutTrans(QPointF &point, const ImageTransform& transform);
	void raletiveToGlobal(QPointF& point, const ImageTexture& image);
	void raletiveToGlobal(QPointF& point, const ImageTransform& transform);
	void raletiveToGlobalWithoutTrans(QPointF& point, const ImageTransform& transform);

	QPointF getRaletiveToRect(const QPointF& point, const ImageTexture& image);
	QPointF getRaletiveToRect(const QPointF& point, const ImageTransform& transform);
	QPointF getRaletiveToRectWithoutTrans(const QPointF& point, const ImageTransform& transform);
	QPointF getRaletiveToGlobal(const QPointF& point, const ImageTexture& image);
	QPointF getRaletiveToGlobal(const QPointF& point, const ImageTransform& transform);
	QPointF getRaletiveToGlobalWithoutTrans(const QPointF& point, const ImageTransform& transform);

	bool isInsideSquare(const QPointF& point, float side = 2.0f);

	MousePos getMousePosType(const QPointF& point, const ImageTexture& image);
	Qt::CursorShape getCursorShape(const MousePos& pos);

	inline QPointF mapToGL(const QPointF& point);

public:
	bool lineMode = true;
	bool drawVisible = true;

	QAction* actionAddBackGround;
	QAction* actionFullScreenBackGround;

private:
	QOpenGLShaderProgram shaderProgram;

	// imageTextures
	QList<ImageTexture> backGrounds;
	QList<ImageTexture> characterTextures;

	int currentSelectedBackGround = -1;

	float aspect;
	float viewScale;

	QMatrix4x4 proj;
	QMatrix4x4 insProj;

	ImageTransform LastImageTransform;

	QPointF LastSelectCenterToMousePos;
	QPointF LastMousePos;

	MousePos LastMousePosType = MousePos::OutSide;
};

