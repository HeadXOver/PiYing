#include "PiYing.h"
#include <QMessageBox>

#include "RatioDialog.h"

PiYing::PiYing(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);
    setWindowTitle("皮影");

    float ratio = 16.0f / 9.0f;

    voidListWidget = new QListWidget();

    bgImageList = new QListWidget();
    chImageList = new QListWidget();
    timeLine = new QWidget(this);

    // OpenGL widget
    piYingGL = new PiYingGL(this);
    piYingGLContainer = new PiYingGLContainer(piYingGL, ratio);

    splitTimelineOpenGL = new QSplitter(Qt::Vertical, this);
    splitListOpenGL = new QSplitter(Qt::Horizontal, this);

    toolList.append(ToolButton(":/PiYing/selectChVert_S.png", ":/PiYing/selectChVert.png", "selectChVert", ToolState::SelectVert));
    toolList.append(ToolButton(":/PiYing/addChVert_S.png", ":/PiYing/addChVert.png", "addChVert", ToolState::AddVert));

    toolList[0].select();

    QComboBox* modeBox = new QComboBox(this);

    // menuBar
    QMenu* menuFile = new QMenu("文件(&F)", this);
    QMenu* menuEdit = new QMenu("编辑(&E)", this);
    ui.menuBar->addMenu(menuFile);
	ui.menuBar->addMenu(menuEdit);

	// child menu of menu File
    QMenu* childMenuImport = menuFile->addMenu("导入");
    QMenu* childMenuExport = menuFile->addMenu("导出");

	// child menu of menu Edit
    QMenu* childMenuScreen = menuEdit->addMenu("幕布");

	// actions of menu File
    QAction* actionExit                  = menuFile->            addAction("退出");
    QAction* actionImportBackGround      = childMenuImport->     addAction("背景图");
    QAction* actionImportCharacter       = childMenuImport->     addAction("角色图");
    QAction* actionExportCurrentFrame    = childMenuExport->     addAction("当前帧");

	// actions of menu Edit
    QAction* actionScreenScale           = childMenuScreen->     addAction("比例...");
    QAction* actionDefaultColor          = childMenuScreen->     addAction("底色...");

    for (ToolButton& item : toolList) {
        ui.mainToolBar->addAction(item.action);
        connect(item.action, &QAction::triggered, this, [this, &item]() {selectTool(item); });
    }

    connect(actionExit,                 SIGNAL(triggered()), this, SLOT(close()));
    connect(actionImportBackGround,     SIGNAL(triggered()), this, SLOT(importBackGround()));
    connect(actionImportCharacter,      SIGNAL(triggered()), this, SLOT(importCharacter()));
    connect(actionExportCurrentFrame,   SIGNAL(triggered()), this, SLOT(exportCurrentFrame()));
	connect(actionScreenScale,          SIGNAL(triggered()), this, SLOT(askScreenScale()));
	connect(actionDefaultColor,         SIGNAL(triggered()), this, SLOT(askDefaultColor()));
    connect(chImageList, &QListWidget::currentItemChanged, this, [this]() {piYingGL->update(); });

    piYingGLContainer->setRatio(ratio);
    piYingGL->changeRatio(ratio);

    splitTimelineOpenGL->addWidget(piYingGLContainer);
    splitTimelineOpenGL->addWidget(timeLine);
    splitListOpenGL->addWidget(voidListWidget);
    splitListOpenGL->addWidget(splitTimelineOpenGL);
    splitTimelineOpenGL->setStretchFactor(0, 1);
    splitTimelineOpenGL->setStretchFactor(1, 2);
    splitListOpenGL->setStretchFactor(0, 1);
    splitListOpenGL->setStretchFactor(1, 3);

    bgImageList->setUniformItemSizes(true);
    chImageList->setUniformItemSizes(true);
    bgImageList->setIconSize(QSize(50, 50));
    chImageList->setIconSize(QSize(50, 50));

    QFile qss(":/PiYing/imageListStyle.qss");
    if (qss.open(QFile::ReadOnly)) {
        bgImageList->setStyleSheet(qss.readAll());
        qss.seek(0);
        chImageList->setStyleSheet(qss.readAll());
        qss.close();
    }

    piYingGL->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    timeLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    voidListWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    modeBox->addItems({ "预览模式", "背景编辑", "角色纹理编辑", "角色骨骼编辑" });
    ui.statusBar->addWidget(modeBox);

    connect(modeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PiYing::onModeChanged);

    onModeChanged(0);

    setCentralWidget(splitListOpenGL);
}

PiYing::~PiYing()
{}

void PiYing::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape) {
        if (piYingGL->editMode == EditMode::characterTexture) {
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
    else if (event->key() == Qt::Key_Delete) {
        if (piYingGL->editMode == EditMode::characterTexture) {
            piYingGL->deleteChVert();
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

        if (r.w == 0 || r.h == 0) return;

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
        piYingGL->setEditMode(EditMode::characterTexture);
    }
    else if (mode == 3) {
        splitListOpenGL->widget(0)->setParent(this);
        splitListOpenGL->insertWidget(0, chImageList);
        piYingGL->setEditMode(EditMode::characterSkeleton);
    }
}

void PiYing::selectTool(ToolButton& toolButton)
{
    for (ToolButton& item : toolList) item.unSelect();
    toolButton.select();
    piYingGL->setToolState(toolButton.toolState);
}

void PiYing::importBackGround(){
    piYingGL->importBackground();
}
