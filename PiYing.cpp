#include "PiYing.h"
#include <QMessageBox>

#include "RatioDialog.h"

PiYing::PiYing(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);
    setWindowTitle("皮影");

    // menuBar
    menuFile = new QMenu("文件(&F)");
	menuEdit = new QMenu("编辑(&E)");
    ui.menuBar->addMenu(menuFile);
	ui.menuBar->addMenu(menuEdit);

	// child menu of menu File
    childMenuImport = menuFile->addMenu("导入");
    childMenuExport = menuFile->addMenu("导出");

	// child menu of menu Edit
	childMenuScreen = menuEdit->addMenu("幕布");

	// actions of menu File
    actionExit                              = menuFile->            addAction("退出");
    actionImportBackGround                  = childMenuImport->     addAction("背景图");
    actionImportCharacter                   = childMenuImport->     addAction("角色图");
    actionExportCurrentFrame                = childMenuExport->     addAction("当前帧");

	// actions of menu Edit
	actionScreenScale           = childMenuScreen->     addAction("比例...");
	actionDefaultColor          = childMenuScreen->     addAction("底色...");

	// signals of actions
    connect(actionExit,                 SIGNAL(triggered()), this, SLOT(close()));
    connect(actionImportBackGround,     SIGNAL(triggered()), this, SLOT(importBackGround()));
    connect(actionImportCharacter,      SIGNAL(triggered()), this, SLOT(importCharacter()));
    connect(actionExportCurrentFrame,   SIGNAL(triggered()), this, SLOT(exportCurrentFrame()));
	connect(actionScreenScale,          SIGNAL(triggered()), this, SLOT(askScreenScale()));
	connect(actionDefaultColor,         SIGNAL(triggered()), this, SLOT(askDefaultColor()));

	// OpenGL widget
    piYingGL = new PiYingGL(this);
	setCentralWidget(piYingGL);

    piYingGLContainer = new PiYingGLContainer(piYingGL, 16.0, 9.0); // 16:9
    setCentralWidget(piYingGLContainer);

	float ratio = 16.0f / 9.0f;
    piYingGLContainer->setRatio(ratio);
    piYingGL->changeRatio(ratio);

    statusBar()->addPermanentWidget(new QLabel(tr("视图x"), this));
    statusBar()->addPermanentWidget(piYingGL->labelViewTransX);
    statusBar()->addPermanentWidget(new QLabel(tr("视图y"), this));
    statusBar()->addPermanentWidget(piYingGL->labelViewTransY);
    statusBar()->addPermanentWidget(new QLabel(tr("视图旋转"), this));
    statusBar()->addPermanentWidget(piYingGL->labelViewRot);
    statusBar()->addPermanentWidget(new QLabel(tr("视图缩放"), this));
    statusBar()->addPermanentWidget(piYingGL->labelViewScale);
}

PiYing::~PiYing()
{}

void PiYing::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key::Key_B) {
        if (piYingGL->editMode == EditMode::BackGround) {
            piYingGL->editMode = EditMode::Default;
        }
        else piYingGL->editMode = EditMode::BackGround;
        piYingGL->currentUpdate();
    }
}

void PiYing::importCharacter(){
    piYingGL->importChatacter();
}

void PiYing::exportCurrentFrame(){
    QString selectedFilter;
    QString path = QFileDialog::getSaveFileName(
        this,
        tr("保存图片"),
        "currentFrame.png",
        tr("PNG Files (*.png);;JPG Files (*.jpg)"),
        &selectedFilter);

    if (path.isEmpty()) return;

    const char* format = nullptr;
    if (selectedFilter.contains("*.png", Qt::CaseInsensitive)) format = "PNG";
    else if (selectedFilter.contains("*.jpg", Qt::CaseInsensitive)) format = "JPG";
    else {
		QMessageBox::warning(this, tr("Error"), tr("不支持的格式"));
        return;
    }

    QImage img = piYingGL->grabFramebuffer();

    if (!img.save(path, format, 95))  QMessageBox::warning(this, tr("Error"), tr("Failed to save"));
}

void PiYing::askScreenScale(){
    RatioDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        Ratio r = dlg.ratio();

        //QMessageBox::warning(this, tr("Info"), QString("比例: %1:%2").arg(r.w).arg(r.h));
		float ratio = float(r.w) / float(r.h);
		piYingGLContainer->setRatio(ratio);
		piYingGLContainer->update();
		piYingGL->changeRatio(ratio);
    }
}

void PiYing::askDefaultColor(){
	piYingGL->choseBackgroundColor();
}

void PiYing::importBackGround(){
    piYingGL->importBackground();
}
