#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PiYing.h"

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QFile>
#include <QList>

#include "cusMode.h"
#include "piYingGL.h"

class PiYing : public QMainWindow
{
    Q_OBJECT

public:
    PiYing(QWidget *parent = nullptr);
    ~PiYing();

private slots:
    void importBackGround();

private:
    Ui::PiYingClass ui;

    // menus
    QMenu* menuFile;

	// child menus of menuFile
	QMenu* childMenuImport;

	// actions of menuFile
    QAction* actionExit;
    QAction* actionImportBackGround;

	// OpenGL widget
    PiYingGL* piYingGL = nullptr;
};

