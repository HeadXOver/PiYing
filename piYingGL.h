#pragma once

#include <QOpenGLWidget.h>
#include <qopenglfunctions_3_3_Core>
#include <QOpenGLShaderProgram.h>
#include <qlabel>
#include <qmenu>

#include "KeyboardStateWin.h"
#include "ViewData.h"
#include "AskTransformDialog.h"
#include "ChElementSelect.h"
#include "AddTriangle.h"
#include "Element.h"
#include "tool_button.h"
#include "image_texture.h"

enum class EditMode {
	Default,
	BackGround,
	characterTexture,
	characterSkeleton,
	controlSlide,
};

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

class PiYing;

class PiYingGL : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
	Q_OBJECT

public:
	explicit PiYingGL(PiYing* parent = nullptr);
	~PiYingGL();

private:
	void paintBackgrounds();
	void paintCharacterTexture();
	void paintCharacterSkeleton();
	void addGlobalAction(QMenu* menu, const QList<QAction*> action) { for (QAction* item : action)  menu->addAction(item); }

	int getCurrentBgRow() const;
	int getCurrentChRow() const;

private slots:
	void fullScreenBackGround();
	void setViewToStandard();
	void returnToStandard();
	void returnBgTransform();
	void bgSetTransform();
	void againstBg();
	void deleteBg();
	void deleteAllBg();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* ev) override;
	void contextMenuEvent(QContextMenuEvent* e) override;

public:
	void addBackground(const QString& imageName);
	void appendBgList(QImage& image);
	void addCharacter(const QString& imageName);
	void setEditMode(EditMode mode);
	void updateChTexTool();
	void setChToolState(ChTexToolState state);
	void deleteChElement();
	void enterChElement();
	void escapeChVert();
	void choseBackgroundColor();
	void changeRatio(float ratio);
	void importBackground();
	void importChatacter();
	void currentUpdate();
	void bgRotationControl(const QPointF& mouse, ImageTexture& image);
	void bgTranslateControl(const QPointF& mouse, ImageTexture& image);
	void bgScaleControl(const QPointF& mouse, ImageTexture& image);
	void viewRotationControl(const QPointF& mouse);
	void drawChEditVert();

	bool addBackground(const QString& imageName, QImage& image);

	QPointF getRaletiveToRect(const QPointF& point, const ImageTransform& transform) const		{ return (proj * transform.getMatrixInvert() * getViewMatrixInvert() * insProj).map(point); }
	QPointF getRaletiveToGlobal(const QPointF& point, const ImageTransform& transform) const	{ return getBgShaderMatrix(transform).map(point); }
	QPointF mapToGL(const QPointF& point) { return QPointF((point.x() / float(width())) * 2.0f - 1.0f, 1.0f - (point.y() / float(height())) * 2.0f); }
	QPointF mapToGL(float x, float y) { return QPointF((x / float(width())) * 2.0f - 1.0f, 1.0f - (y / float(height())) * 2.0f); }
	QPointF glToMap(const QPointF& point) { return QPointF((point.x() + 1.0f) * width() / 2, (1.0f - point.y()) * height() / 2); }
	QPointF mapViewProjMatrix(const QPointF& point) { return glToMap(getViewProjMatrix().map(point)); }
	QPointF GLViewProjMatrixInvert(float x, float y) { return getViewProjMatrixInvert().map(mapToGL(x, y)); }
	QPointF GLViewProjMatrixInvert(const QPointF& point) { return getViewProjMatrixInvert().map(mapToGL(point)); }

	QMatrix4x4 getViewMatrixInvert() const;
	QMatrix4x4 getViewMatrix() const;
	QMatrix4x4 getBgShaderMatrix(const ImageTransform& transform) const { return proj * getViewMatrix() * transform.getMatrix() * insProj; }
	QMatrix4x4 getViewProjMatrixInvert() const { return proj * getViewMatrixInvert() * insProj; }
	QMatrix4x4 getViewProjMatrix() const { return proj * getViewMatrix() * insProj; }
	QMatrix4x4 getProj() const { return proj; }
	QMatrix4x4 getInsProj() const {return insProj; }

	bool isInsideSquare(const QPointF& point, float side = 2.0f) const { return (point.x() >= -side / 2.f && point.x() <= side / 2.f && point.y() >= -side / 2.f && point.y() <= side / 2.f); }

	MousePos getMousePosType(const QPointF& point) const;

	Qt::CursorShape getCursorShape(const MousePos& pos);

	QList<std::vector<float>>& ref_characterVerts() { return characterVerts; }
	QList<QList<QPointF>>& ref_characterDrawVerts() { return characterVertsUV; }
	QList<std::vector<unsigned int>>& ref_characterTriangleIndices () { return characterTriangleIndices; }

public:
	EditMode editMode = EditMode::Default;

	ViewData viewScale;
	ViewData viewRotate;
	ViewData viewTransX;
	ViewData viewTransY;

private:
	PiYing* parent;

	unsigned int bgVAO = 0, bgVBO = 0, bgEBO = 0;
	unsigned int chVAO = 0, chVBO = 0, chEBO = 0;

	QOpenGLShaderProgram bgShaderProgram;
	QOpenGLShaderProgram chShaderProgram;

	// imageTextures
	QList<ImageTexture> backGrounds;
	QList<ImageTexture> characterTextures;

	QList<std::vector<float>> characterVerts;
	QList<QList<QPointF>> characterVertsUV;
	QList<std::vector<unsigned int>> characterTriangleIndices;

	float lastViewRotate = 0.f;
	float lastViewTransX = 0.f;
	float lastViewTransY = 0.f;

	QMatrix4x4 proj;
	QMatrix4x4 insProj;

	ImageTransform lastImageTransform;

	QPointF lastMousePos;
	QPointF lastMiddleButtonPos;

	MousePos lastMousePosType = MousePos::OutSide;

	ChTexToolState chToolState;

	QMenu* rightButtonMenuChTex;
	QMenu* rightButtonMenuBg_S;
	QMenu* rightButtonMenuBg;
	QMenu* rightButtonMenu;

	ChElementTool* chElementTool;
};