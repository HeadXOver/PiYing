#pragma once

#include <QOpenGLWidget.h>
#include <qopenglfunctions_3_3_Core>
#include <QOpenGLShaderProgram.h>

#include "KeyboardStateWin.h"
#include "ViewData.h"
#include "AskTransformDialog.h"
#include "ChElementSelect.h"

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
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* ev) override;
	void contextMenuEvent(QContextMenuEvent* e) override;

public:
	void addBackground(const QString& imageName);
	void appendBgList(QImage& image);
	void addCharacter(const QString& imageName);
	void addChVert(const QPointF& point, int currentVector);
	void setEditMode(EditMode mode);
	void setChToolState(ChToolState state);
	void deleteChVert();
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
	void chAddTriangleControl(const QPointF& mouse);
	void drawChEditVert();

	bool addBackground(const QString& imageName, QImage& image);

	inline QPointF getRaletiveToRect(const QPointF& point, const ImageTransform& transform) const	{ return (proj * transform.getMatrixInvert() * getViewMatrixInvert() * insProj).map(point); }
	inline QPointF getRaletiveToGlobal(const QPointF& point, const ImageTransform& transform) const		{ return getBgShaderMatrix(transform).map(point); }

	inline QPointF mapToGL(const QPointF& point) { return QPointF((point.x() / float(width())) * 2.0f - 1.0f, 1.0f - (point.y() / float(height())) * 2.0f); }
	inline QPointF glToMap(const QPointF& point) { return QPointF((point.x() + 1.0f) * width() / 2, (1.0f - point.y()) * height() / 2); }
	inline QPointF mapViewProjMatrix(const QPointF& point) { return glToMap(getViewProjMatrix().map(mapToGL(point))); }

	QMatrix4x4 getViewMatrixInvert() const;
	QMatrix4x4 getViewMatrix() const;
	QMatrix4x4 getViewMatrixInvertWithoutTrans() const;
	QMatrix4x4 getViewMatrixWithoutTrans() const;
	QMatrix4x4 getBgShaderMatrix(const ImageTransform& transform) const { return proj * getViewMatrix() * transform.getMatrix() * insProj; }

	inline QMatrix4x4 getViewProjMatrixInvert() const { return proj * getViewMatrixInvert() * insProj; }
	inline QMatrix4x4 getViewProjMatrix() const { return proj * getViewMatrix() * insProj; }

	static QString getUniquebgName(const QListWidget* list);

	inline bool isInsideSquare(const QPointF& point, float side = 2.0f) const { return (point.x() >= -side / 2.f && point.x() <= side / 2.f && point.y() >= -side / 2.f && point.y() <= side / 2.f); }

	MousePos getMousePosType(const QPointF& point) const;

	Qt::CursorShape getCursorShape(const MousePos& pos);

public:
	bool lineMode = true;
	bool drawVisible = true;

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
	QList<std::vector<unsigned int>> characterTriangleIndices;

	int currentSelectedBackGround = -1;

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

	ChToolState chToolState;

	QMenu* rightButtonMenuChTex;
	QMenu* rightButtonMenuBg_S;
	QMenu* rightButtonMenuBg;
	QMenu* rightButtonMenu;

	ChElementTool* chElementTool;
};