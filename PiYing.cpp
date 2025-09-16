#include "PiYing.h"

PiYing::PiYing(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);

    menuFile = new QMenu("文件(&F)");

    ui.menuBar->addMenu(menuFile);

    actionExit = new QAction("退出");

    menuFile->addAction(actionExit);

    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
}

PiYing::~PiYing()
{}

