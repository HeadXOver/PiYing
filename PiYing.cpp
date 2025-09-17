#include "PiYing.h"

PiYing::PiYing(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);

    setWindowTitle("皮影");

    menuFile = new QMenu("文件(&F)");
    ui.menuBar->addMenu(menuFile);

    actionExit = new QAction("退出");
    menuFile->addAction(actionExit);

    piYingGL = new PiYingGL(this);
    setCentralWidget(piYingGL);

    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
}

PiYing::~PiYing()
{}

