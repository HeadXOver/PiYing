#pragma once

#include <QtWidgets/QMainWindow>
#include <QToolBar>
#include <QComboBox>
#include "ui_PiYing.h"

#include "cusMode.h"
#include "piYingGL.h"
#include "piYingGLContainer.h"

class PiYing : public QMainWindow
{
    Q_OBJECT

public:
    PiYing(QWidget *parent = nullptr);
    ~PiYing();

protected:
	void keyPressEvent(QKeyEvent* event) override;

private slots:
	void importCharacter();
	void importBackGround();
    void exportCurrentFrame();
	void askScreenScale();
	void askDefaultColor();
	void onModeChanged(int mode);

private:
	void selectTool(ToolButton& toolButton);

private:
    Ui::PiYingClass ui;

	// OpenGL widget
    PiYingGL* piYingGL = nullptr;

	QSplitter* splitListOpenGL;
	QSplitter* splitTimelineOpenGL;

	QWidget* timeLine;

	QList<ToolButton> toolChTexList;

public:
	PiYingGLContainer* piYingGLContainer = nullptr;

	QListWidget* bgImageList = nullptr;
	QListWidget* chImageList = nullptr;
	QListWidget* voidListWidget;
};

