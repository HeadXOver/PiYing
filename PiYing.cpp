#include "PiYing.h"
#include <QMessageBox>

PiYing::PiYing(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);
    setWindowTitle("皮影");

    // menu File
    menuFile = new QMenu("文件(&F)");
    ui.menuBar->addMenu(menuFile);

	// child menu of menu File
    childMenuImport = menuFile->addMenu("导入");

	// actions of menu File
    actionExit                  = menuFile->            addAction("退出");
    actionImportBackGround      = childMenuImport->     addAction("背景图");

	// signals of actions
    connect(actionExit,             SIGNAL(triggered()), this, SLOT(close()));
    connect(actionImportBackGround, SIGNAL(triggered()), this, SLOT(importBackGround()));

	// OpenGL widget
    piYingGL = new PiYingGL(this);
    setCentralWidget(piYingGL);
}

PiYing::~PiYing()
{}

void PiYing::importBackGround() {
    QString fileName = QFileDialog::getOpenFileName(this, "选择背景图", ".", "Images (*.png *.xpm *.jpg)");
    if (fileName.isEmpty()) {
        return;
    }
	piYingGL->addBackground(fileName);
}

