#pragma once

#include <QOpenGLWidget.h>
#include <qopenglfunctions_3_3_Core>
#include <QOpenGLShaderProgram.h>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>

#include "Element.h"
#include "KeyboardStateWin.h"

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

enum class EditMode {
	Default,
	BackGround
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
	void choseBackGroundColor();
	void setViewToStandard();
	void returnToStandard();

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
	void choseBackgroundColor();
	void changeRatio(float ratio);
	void importBackground();
	void currentUpdate();
	void bgRotationControl(const QPointF& mouse, ImageTexture& image);
	void bgTranslateControl(const QPointF& mouse, ImageTexture& image);
	void bgScaleControl(const QPointF& mouse, ImageTexture& image);

	inline void raletiveToRect(QPointF& point, const ImageTexture& image)						{ point = getRaletiveToRect(point, image); }
	inline void raletiveToRect(QPointF& point, const ImageTransform& transform)					{ point = getRaletiveToRect(point, transform); }
	inline void raletiveToRectWithoutTrans(QPointF& point, const ImageTransform& transform)		{ point = getRaletiveToRectWithoutTrans(point, transform); }
	inline void raletiveToGlobal(QPointF& point, const ImageTexture& image)						{ point = getRaletiveToGlobal(point, image); }
	inline void raletiveToGlobal(QPointF& point, const ImageTransform& transform)				{ point = getRaletiveToGlobal(point, transform); }
	inline void raletiveToGlobalWithoutTrans(QPointF& point, const ImageTransform& transform)	{ point = getRaletiveToGlobalWithoutTrans(point, transform); }

	inline QPointF getRaletiveToRect(const QPointF& point, const ImageTexture& image) const							{ return getRaletiveToRect(point, { image.trans, image.rot,image.scale }); }
	inline QPointF getRaletiveToRect(const QPointF& point, const ImageTransform& transform) const					{ return (proj * transform.scale.inverted() * transform.rot.inverted() * transform.trans.inverted() * getViewMatrixInvert() * insProj).map(point); }
	inline QPointF getRaletiveToRectWithoutTrans(const QPointF& point, const ImageTransform& transform) const		{ return (proj * transform.scale.inverted() * transform.rot.inverted() * getViewMatrixInvert() * insProj).map(point); }
	inline QPointF getRaletiveToRectWithoutTransProj(const QPointF& point, const ImageTransform& transform) const	{ return (transform.scale.inverted() * transform.rot.inverted() * getViewMatrixInvert() * insProj).map(point); }
	inline QPointF getRaletiveToGlobal(const QPointF& point, const ImageTexture& image) const						{ return getRaletiveToGlobal(point, { image.trans, image.rot,image.scale }); }
	inline QPointF getRaletiveToGlobal(const QPointF& point, const ImageTransform& transform) const					{ return (proj * getViewMatrix() * transform.trans * transform.rot * transform.scale * insProj).map(point); }
	inline QPointF getRaletiveToGlobalWithoutTrans(const QPointF& point, const ImageTransform& transform) const		{ return (proj * getViewMatrix() * transform.rot * transform.scale * insProj).map(point); }
	inline QPointF getRaletiveToGlobalWithoutTransProj(const QPointF& point, const ImageTransform& transform) const	{ return (getViewMatrix() * transform.rot * transform.scale * insProj).map(point); }

	inline QPointF mapToGL(const QPointF& point) { return QPointF((point.x() / float(width())) * 2.0f - 1.0f, 1.0f - (point.y() / float(height())) * 2.0f); }

	QMatrix4x4 getViewMatrixInvert() const;
	QMatrix4x4 getViewMatrix() const;

	bool isInsideSquare(const QPointF& point, float side = 2.0f);

	MousePos getMousePosType(const QPointF& point, const ImageTexture& image);

	Qt::CursorShape getCursorShape(const MousePos& pos);

public:
	bool lineMode = true;
	bool drawVisible = true;

	QAction* actionAddBackGround;
	QAction* actionFullScreenBackGround;
	QAction* actionChoseBackGroundColor;
	QAction* actionSetViewToStandard;
	QAction* actionReturnToStandard;

	EditMode editMode = EditMode::Default;

private:
	unsigned int VAO = 0, VBO = 0, EBO = 0;

	QOpenGLShaderProgram shaderProgram;

	// imageTextures
	QList<ImageTexture> backGrounds;
	QList<ImageTexture> characterTextures;

	int currentSelectedBackGround = -1;

	float aspect;
	float viewScale;
	float viewRotate = 0.f;
	float viewTransX = 0.f;
	float viewTransY = 0.f;

	QMatrix4x4 proj;
	QMatrix4x4 insProj;

	ImageTransform LastImageTransform;

	QPointF LastMousePos;
	QPointF LastMiddleButtonPos;

	MousePos LastMousePosType = MousePos::OutSide;

	QColor backGroundColor;

	QPen framePen = QPen(Qt::red);
};

