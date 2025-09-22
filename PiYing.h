#pragma once

#include <QtWidgets/QMainWindow>
#include <QToolBar>
#include <QComboBox>
#include "ui_PiYing.h"

#include "cusMode.h"
#include "piYingGL.h"
#include "piYingGLContainer.h"

QT_BEGIN_NAMESPACE
class QSpinBox;
class QDialogButtonBox;
QT_END_NAMESPACE

class PiYing : public QMainWindow
{
    Q_OBJECT

public:
    PiYing(QWidget *parent = nullptr);
    ~PiYing();

private slots:
	void importCharacter();
	void importBackGround();
    void exportCurrentFrame();
	void askScreenScale();
	void askDefaultColor();
	void onModeChanged(int mode);

private:
    Ui::PiYingClass ui;

    // menus
    QMenu* menuFile;
	QMenu* menuEdit;

	// child menus of menuFile
	QMenu* childMenuImport;
    QMenu* childMenuExport;

	// child menus of menuEdit
	QMenu* childMenuScreen;

	// actions of menuFile
    QAction* actionExit;
	QAction* actionImportCharacter;
    QAction* actionExportCurrentFrame;
	QAction* actionImportBackGround;

	// actions of menuEdit
	QAction* actionScreenScale;
	QAction* actionDefaultColor;

	// OpenGL widget
    PiYingGL* piYingGL = nullptr;

	QSplitter* splitListOpenGL;
	QSplitter* splitTimelineOpenGL;

	QWidget* timeLine;

	QComboBox* modeBox;

public:
	PiYingGLContainer* piYingGLContainer = nullptr;

	QListWidget* bgImageList = nullptr;
};

