#pragma once

#include <QOpenGLWidget.h>
#include <qopenglfunctions_3_3_Core>
#include <QOpenGLShaderProgram.h>

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>
#include <QStatusBar>
#include <QLabel>
#include <QList>
#include <QListWidget>
#include <QSplitter>

#include "Element.h"
#include "KeyboardStateWin.h"
#include "ViewData.h"
#include "AskTransformDialog.h"

class PiYing;

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
	BackGround,
	characterOverView
};

class PiYingGL : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
	Q_OBJECT

public:
	explicit PiYingGL(PiYing* parent = nullptr);
	~PiYingGL();

private:
	void paintBackgrounds();
	void paintCharactersOverView();

private slots:
	void fullScreenBackGround();
	void setViewToStandard();
	void returnToStandard();
	void returnbgTransform();
	void bgSetTransform();
	void againstBg();
	void deleteBg();
	void deleteAllBg();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* ev) override;
	void contextMenuEvent(QContextMenuEvent* e) override;

public:
	void addBackground(const QString& imageName);
	void appendBgList(QImage& image);
	void addCharacter(const QString& imageName);
	void setEditMode(EditMode mode);
	void choseBackgroundColor();
	void changeRatio(float ratio);
	void updateProjMatrix();
	void importBackground();
	void importChatacter();
	void currentUpdate();
	void bgRotationControl(const QPointF& mouse, ImageTexture& image);
	void bgTranslateControl(const QPointF& mouse, ImageTexture& image);
	void bgScaleControl(const QPointF& mouse, ImageTexture& image);
	void viewRotationControl(const QPointF& mouse);

	bool addBackground(const QString& imageName, QImage& image);

	inline QPointF getRaletiveToRect(const QPointF& point, const ImageTransform& transform) const	{ return (proj * transform.getMatrixInvert() * getViewMatrixInvert() * insProj).map(point); }
	inline QPointF getRaletiveToGlobal(const QPointF& point, const ImageTransform& transform) const		{ return getBgShaderMatrix(transform).map(point); }

	inline QPointF mapToGL(const QPointF& point) { return QPointF((point.x() / float(width())) * 2.0f - 1.0f, 1.0f - (point.y() / float(height())) * 2.0f); }

	QMatrix4x4 getViewMatrixInvert() const;
	QMatrix4x4 getViewMatrix() const;
	QMatrix4x4 getViewMatrixInvertWithoutTrans() const;
	QMatrix4x4 getViewMatrixWithoutTrans() const;
	QMatrix4x4 getBgShaderMatrix(const ImageTransform& transform) const;

	static QString getUniquebgName(const QListWidget* list);

	inline bool isInsideSquare(const QPointF& point, float side = 2.0f) const { return (point.x() >= -side / 2.f && point.x() <= side / 2.f && point.y() >= -side / 2.f && point.y() <= side / 2.f); }

	MousePos getMousePosType(const QPointF& point) const;

	Qt::CursorShape getCursorShape(const MousePos& pos);

public:
	bool lineMode = true;
	bool drawVisible = true;

	QAction* actionAddBackGround;
	QAction* actionAddCharacterTexture;
	QAction* actionFullScreenBackGround;
	QAction* actionChoseBackGroundColor;
	QAction* actionSetViewToStandard;
	QAction* actionReturnToStandard;
	QAction* actionReturnbgTransform;
	QAction* actionDeleteBg;
	QAction* actionDeleteAllBg;
	QAction* actionBgSetTransform;
	QAction* actionAgainstBg;

	EditMode editMode = EditMode::Default;

	ViewData viewScale;
	ViewData viewRotate;
	ViewData viewTransX;
	ViewData viewTransY;

	QLabel* labelViewScale;
	QLabel* labelViewTransX;
	QLabel* labelViewTransY;
	QLabel* labelViewRot;

private:
	unsigned int bgVAO = 0, bgVBO = 0, bgEBO = 0;
	unsigned int chVAO = 0, chVBO = 0, chEBO = 0;

	QOpenGLShaderProgram bgShaderProgram;
	QOpenGLShaderProgram chShaderProgram;

	// imageTextures
	QList<ImageTexture> backGrounds;
	QList<ImageTexture> characterTextures;

	int currentSelectedBackGround = -1;

	float aspect;
	float lastViewRotate = 0.f;
	float lastViewTransX = 0.f;
	float lastViewTransY = 0.f;

	QMatrix4x4 proj;
	QMatrix4x4 insProj;

	ImageTransform lastImageTransform;

	QPointF lastMousePos;
	QPointF lastMiddleButtonPos;

	MousePos lastMousePosType = MousePos::OutSide;

	QColor backGroundColor;

	QPen framePen = QPen(Qt::red);

	PiYing* parent;

	std::vector<float> characterVerts;
	std::vector<unsigned int> characterIndices;
};

