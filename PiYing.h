#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PiYing.h"

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QList>

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
    void importBackGround();
    void importCharacter();
    void exportCurrentFrame();
	void askScreenScale();
	void askDefaultColor();

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
    QAction* actionImportBackGround;
	QAction* actionImportCharacter;
    QAction* actionExportCurrentFrame;

	// actions of menuEdit
	QAction* actionScreenScale;
	QAction* actionDefaultColor;

	// OpenGL widget
    PiYingGL* piYingGL = nullptr;
	PiYingGLContainer* piYingGLContainer = nullptr;
};

