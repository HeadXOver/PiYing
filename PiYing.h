#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PiYing.h"

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

protected:
	void keyPressEvent(QKeyEvent* event) override;

private slots:
	void importCharacter();
	void importBackGround();
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
	QAction* actionImportCharacter;
    QAction* actionExportCurrentFrame;
	QAction* actionImportBackGround;

	// actions of menuEdit
	QAction* actionScreenScale;
	QAction* actionDefaultColor;

	// OpenGL widget
    PiYingGL* piYingGL = nullptr;
	PiYingGLContainer* piYingGLContainer = nullptr;
};

