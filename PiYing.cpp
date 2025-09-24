#include "PiYing.h"
#include <QMessageBox>

#include "RatioDialog.h"

PiYing::PiYing(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);
    setWindowTitle("皮影");

    voidListWidget = new QListWidget();

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

    float ratio = 16.0f / 9.0f;

	// OpenGL widget
    piYingGL = new PiYingGL(this);
    piYingGLContainer = new PiYingGLContainer(piYingGL, ratio); // 16:9

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

    bgImageList = new QListWidget();
    chImageList = new QListWidget();
    timeLine = new QWidget(this);

    splitTimelineOpenGL = new QSplitter(Qt::Vertical, this);
    splitListOpenGL = new QSplitter(Qt::Horizontal, this);

    splitTimelineOpenGL->addWidget(piYingGLContainer);
    splitTimelineOpenGL->addWidget(timeLine);
    splitListOpenGL->addWidget(voidListWidget);
    splitListOpenGL->addWidget(splitTimelineOpenGL);
    splitTimelineOpenGL->setStretchFactor(0, 1);
    splitTimelineOpenGL->setStretchFactor(1, 2);
    splitListOpenGL->setStretchFactor(0, 1);
    splitListOpenGL->setStretchFactor(1, 3);

    bgImageList->setUniformItemSizes(true);
    bgImageList->setIconSize(QSize(50, 50));
    bgImageList->setStyleSheet(
        "QListWidget { background-color: rgb(35, 8, 8); }"
        "QListWidget::item { background-color: rgb(255, 253, 226);}"
        "QListWidget::item:selected { background-color: rgb(112, 112, 112); }"
    );

    chImageList->setUniformItemSizes(true);
    chImageList->setIconSize(QSize(50, 50));
    chImageList->setStyleSheet(
        "QListWidget { background-color: rgb(35, 8, 8); }"
        "QListWidget::item { background-color: rgb(255, 253, 226);}"
        "QListWidget::item:selected { background-color: rgb(112, 112, 112); }"
    );

    piYingGL->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    timeLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    voidListWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    modeBox = new QComboBox;
    modeBox->addItems({ "预览模式", "背景编辑", "角色编辑"});
    ui.mainToolBar->addWidget(modeBox);

    connect(modeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PiYing::onModeChanged);

    onModeChanged(0);

    setCentralWidget(splitListOpenGL);
}

PiYing::~PiYing()
{}

void PiYing::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape) {
        if (piYingGL->editMode == EditMode::characterOverView) {
            if (piYingGL->first2VertState == First2VertState::None) return;
            if (piYingGL->first2VertState == First2VertState::HalfSelect || piYingGL->first2VertState == First2VertState::HalfPoint)
                piYingGL->first2VertState = First2VertState::None;
            else if (piYingGL->first2VertState == First2VertState::Full2Point) piYingGL->first2VertState = First2VertState::HalfPoint;
            else if (piYingGL->first2VertState == First2VertState::Full2Select) piYingGL->first2VertState = First2VertState::HalfSelect;
            else if (piYingGL->first2VertState == First2VertState::FullSelectPoint) piYingGL->first2VertState = First2VertState::HalfSelect;
            else if (piYingGL->first2VertState == First2VertState::FullPointSelect) piYingGL->first2VertState = First2VertState::HalfPoint;

            piYingGL->update();
        }
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

void PiYing::onModeChanged(int mode)
{
    if (mode == 0) {
        splitListOpenGL->widget(0)->setParent(this);
        splitListOpenGL->insertWidget(0, voidListWidget);

        piYingGL->setEditMode(EditMode::Default);
    }
    else if(mode == 1){
        splitListOpenGL->widget(0)->setParent(this);
        splitListOpenGL->insertWidget(0, bgImageList);
        piYingGL->setEditMode(EditMode::BackGround);
    }
    else if (mode == 2) {
        splitListOpenGL->widget(0)->setParent(this);
        splitListOpenGL->insertWidget(0, chImageList);
        piYingGL->setEditMode(EditMode::characterOverView);
    }
}

void PiYing::importBackGround(){
    piYingGL->importBackground();
}
