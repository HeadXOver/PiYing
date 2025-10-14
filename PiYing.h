#pragma once

#include <QtWidgets/QMainWindow>
#include <QToolBar>
#include <QComboBox>
#include <QListWidget>
#include <qsplitter>

#include "ui_PiYing.h"

#include "cusMode.h"
#include "piYingGL.h"
#include "piYingGLContainer.h"

class CtrlSlideWidget;

class PiYing : public QMainWindow
{
    Q_OBJECT

public:
    PiYing(QWidget *parent = nullptr);
    ~PiYing();

public:
	int getCurrentBgRow() { return bgImageList->currentRow(); }
	int getCurrentChRow() { return chImageList->currentRow(); }

protected:
	void keyPressEvent(QKeyEvent* event) override;

private slots:
    void exportCurrentFrame();
	void askScreenScale();
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

	CtrlSlideWidget* sliderWidget = nullptr;
};

