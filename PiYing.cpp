#include "PiYing.h"
#include "ctrlSlideWidget.h"
#include "RatioDialog.h"

#include <QMessageBox>
#include <qkeyevent>
#include <qfiledialog.h>

PiYing::PiYing(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);
    setWindowTitle("皮影");
    setFocusPolicy(Qt::StrongFocus);

    float ratio = 16.0f / 9.0f;

    voidListWidget = new QListWidget();
    bgImageList = new QListWidget();
    chImageList = new QListWidget();
    sliderWidget = new CtrlSlideWidget();
    timeLine = new QWidget(this);

    // OpenGL widget
    piYingGL = new PiYingGL(this);
    piYingGLContainer = new PiYingGLContainer(piYingGL, ratio, this);

    splitTimelineOpenGL = new QSplitter(Qt::Vertical, this);
    splitListOpenGL = new QSplitter(Qt::Horizontal, this);

    toolChTexList.append(ToolButton(":/PiYing/selectRectChVert_S.png", ":/PiYing/selectRectChVert.png", "selectRectChVert", ChTexToolState::RectSelectVert, this));
    toolChTexList.append(ToolButton(":/PiYing/addChVert_S.png", ":/PiYing/addChVert.png", "addChVert", ChTexToolState::AddTriangle, this));
    toolChTexList.append(ToolButton(":/PiYing/chAddPoly_S.png", ":/PiYing/chAddPoly.png", "chAddPoly", ChTexToolState::AddPoly, this));

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

    for (ToolButton& item : toolChTexList) {
        connect(item.action(), &QAction::triggered, this, [this, &item]() {selectTool(item); });
    }

    connect(actionExportCurrentFrame,   SIGNAL(triggered()), this, SLOT(exportCurrentFrame()));
	connect(actionScreenScale,          SIGNAL(triggered()), this, SLOT(askScreenScale()));
    connect(actionDefaultColor,     &QAction::triggered,                this, [this]() {piYingGL->choseBackgroundColor(); });
    connect(actionExit,             &QAction::triggered,                this, [this]() {close(); });
    connect(actionImportBackGround, &QAction::triggered,                this, [this]() {piYingGL->importBackground(); });
    connect(actionImportCharacter,  &QAction::triggered,                this, [this]() {piYingGL->importChatacter(); });
    connect(chImageList,            &QListWidget::currentItemChanged,   this, [this]() {piYingGL->updateChTexTool(); });
    connect(bgImageList,            &QListWidget::currentItemChanged,   this, [this]() {piYingGL->update(); });

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

    qss.setFileName(":/PiYing/slideStyle.qss");
    if (qss.open(QFile::ReadOnly)) {
        sliderWidget->setStyleSheet(qss.readAll());
        qss.close();
    }

    piYingGL->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    timeLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    voidListWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sliderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    modeBox->addItems({ "预览模式", "背景编辑", "角色纹理编辑", "角色骨骼编辑", "控制器"});
    ui.statusBar->addWidget(modeBox);

    connect(modeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PiYing::onModeChanged);

    onModeChanged(0);

    setCentralWidget(splitListOpenGL);
}

PiYing::~PiYing()
{
    delete bgImageList;
    delete chImageList;
    delete sliderWidget;
    delete voidListWidget;
}

void PiYing::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape) {
        if (piYingGL->editMode == EditMode::characterTexture) {
            if (getCurrentChRow() < 0) return;
            piYingGL->escapeChVert();
        }
    }
    else if (event->key() == Qt::Key_Delete) {
        if (piYingGL->editMode == EditMode::characterTexture) {
            if (getCurrentChRow() < 0) return;
            piYingGL->deleteChElement();
        }
    }
    else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        if (piYingGL->editMode == EditMode::characterTexture) {
            if (getCurrentChRow() < 0) return;
            piYingGL->enterChElement();
        }
    }
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

void PiYing::onModeChanged(int mode)
{
    ui.mainToolBar->clear();
    if (mode == 0) {
        splitListOpenGL->widget(0)->setParent(nullptr);
        splitListOpenGL->insertWidget(0, voidListWidget);

        piYingGL->setEditMode(EditMode::Default);
    }
    else if(mode == 1){
        splitListOpenGL->widget(0)->setParent(nullptr);
        splitListOpenGL->insertWidget(0, bgImageList);
        piYingGL->setEditMode(EditMode::BackGround);
    }
    else if (mode == 2) {
        splitListOpenGL->widget(0)->setParent(nullptr);
        splitListOpenGL->insertWidget(0, chImageList);
        piYingGL->setEditMode(EditMode::characterTexture);
        for (ToolButton& item : toolChTexList) {
            ui.mainToolBar->addAction(item.action());
        }
    }
    else if (mode == 3) {
        splitListOpenGL->widget(0)->setParent(nullptr);
        splitListOpenGL->insertWidget(0, chImageList);
        piYingGL->setEditMode(EditMode::characterSkeleton);
    }
    else if (mode == 4) {
        splitListOpenGL->widget(0)->setParent(nullptr);
        splitListOpenGL->insertWidget(0, sliderWidget);
        piYingGL->setEditMode(EditMode::controlSlide);
        splitListOpenGL->setSizes({ width() / 5, width() * 4 / 5 });
    }
}

void PiYing::selectTool(ToolButton& toolButton)
{
    if (toolButton.isSelect()) {
        if (toolButton.toolState() == ChTexToolState::RectSelectVert) {
            toolButton.set_selected(QIcon(":/PiYing/selectLibreChVert_S.png"));
            toolButton.set_unselected(QIcon(":/PiYing/selectLibreChVert.png"));
            toolButton.set_toolState(ChTexToolState::LibreSelectVert);
        }
        else if (toolButton.toolState() == ChTexToolState::LibreSelectVert) {
            toolButton.set_selected(QIcon(":/PiYing/selectRectChVert_S.png"));
            toolButton.set_unselected(QIcon(":/PiYing/selectRectChVert.png"));
            toolButton.set_toolState(ChTexToolState::RectSelectVert);
        }
        else return;
    }
    else for (ToolButton& item : toolChTexList) item.unSelect();

    toolButton.select();
    piYingGL->setChToolState(toolButton.toolState());
}