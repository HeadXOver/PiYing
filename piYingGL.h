#pragma once

#include <QOpenGLWidget>
#include <qopenglfunctions_3_3_Core>
#include <qpoint>
#include <qmatrix4x4>

#include "enum_character_texture_tool_state.h"
#include "static_rect_vert.h"
#include "ViewData.h"

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
class ImageTransform;
class ImageTexture;
class QMenu;
class ChElementTool;
class QOpenGLShaderProgram;
class PointVector;

class PiYingGL : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
	Q_OBJECT

public:
	explicit PiYingGL(PiYing& parent);
	~PiYingGL();

private:
	void paintBackgrounds();
	void paintCharacterTexture();
	void paintCharacterSkeleton();
	void addGlobalAction(QMenu* menu, const QList<QAction*> action);

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
	void updateChTool();
	void setChToolState(CharacterToolState state);
	void deleteChElement();
	void enterChElement();
	void escapeChVert();
	void choseBackgroundColor();
	void changeRatio(float ratio);
	void importBackground();
	void importChatacter();
	void currentUpdate();
	void bgRotationControl(const QPointF& mouse, ImageTexture* image);
	void bgTranslateControl(const QPointF& mouse, ImageTexture* image);
	void bgScaleControl(const QPointF& mouse, ImageTexture* image);
	void viewRotationControl(const QPointF& mouse);
	void drawChEditVert();
	void drawChSkeleVert();

	bool addBackground(const QString& imageName, QImage& image);

	QPointF getRaletiveToRect(const QPointF& point, const ImageTransform* transform) const;
	QPointF getRaletiveToGlobal(const QPointF& point, const ImageTransform* transform) const;
	QPointF mapToGL(const QPointF& point);
	QPointF mapToGL(float x, float y);
	QPointF glToMap(const QPointF& point);
	QPointF mapViewProjMatrix(const QPointF& point);
	QPointF GLViewProjMatrixInvert(float x, float y);
	QPointF GLViewProjMatrixInvert(const QPointF& point);
	QPointF GLViewMatrixInvert(const QPointF& point);

	QMatrix4x4 getViewMatrixInvert() const;
	QMatrix4x4 getViewMatrix() const;
	QMatrix4x4 getBgShaderMatrix(const ImageTransform* transform) const;
	QMatrix4x4 getViewProjMatrixInvert() const;
	QMatrix4x4 getViewProjMatrix() const;
	QMatrix4x4 getProj() const;
	QMatrix4x4 getInsProj() const;

	bool isInsideSquare(const QPointF& point, float side = 2.0f) const { return (point.x() >= -side / 2.f && point.x() <= side / 2.f && point.y() >= -side / 2.f && point.y() <= side / 2.f); }

	MousePos getMousePosType(const QPointF& point) const;

	Qt::CursorShape getCursorShape(const MousePos& pos);

	QList<PointVector*>& ref_characterVerts() { return characterVerts; }
	PointVector& ref_characterVerts(int index) { return *(characterVerts[index]); }
	QList<std::vector<unsigned int>>& ref_characterTriangleIndices () { return characterTriangleIndices; }

	CharacterToolState getChToolState() const { return chToolState; }

public:
	EditMode editMode = EditMode::Default;

	ViewData viewScale;
	ViewData viewRotate;
	ViewData viewTransX;
	ViewData viewTransY;

private:
	PiYing& ref_PiYing;

	unsigned int bgVAO = 0, bgVBO = 0, bgEBO = 0;
	unsigned int chVAO = 0, chVBO = 0, chEBO = 0;

	QOpenGLShaderProgram* bgShaderProgram;
	QOpenGLShaderProgram* chShaderProgram;

	// imageTextures
	QList<ImageTexture*> backGrounds;
	QList<ImageTexture*> characterTextures;

	QList<PointVector*> characterVerts;
	QList<std::vector<unsigned int>> characterTriangleIndices;

	float lastViewRotate = 0.f;
	float lastViewTransX = 0.f;
	float lastViewTransY = 0.f;

	QMatrix4x4 proj;
	QMatrix4x4 insProj;

	ImageTransform* lastImageTransform;

	QPointF lastMousePos;
	QPointF lastMiddleButtonPos;

	MousePos lastMousePosType = MousePos::OutSide;

	CharacterToolState chToolState;

	QMenu* rightButtonMenuChTex;
	QMenu* rightButtonMenuBg_S;
	QMenu* rightButtonMenuBg;
	QMenu* rightButtonMenu;

	ChElementTool* chElementTool;
};