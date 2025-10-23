#include "PiYing.h"
#include "ctrlSlideWidget.h"
#include "RatioDialog.h"
#include "piYingGL.h"
#include "piYingGLContainer.h"
#include "tool_button.h"
#include "time_line_gl.h"
#include "slide_applier.h"

#include "ui_PiYing.h"

#include <QMessageBox>
#include <qkeyevent>
#include <qfiledialog.h>
#include <qlistwidget>
#include <QComboBox>
#include <qsplitter>

PiYing::PiYing(QWidget* parent) : QMainWindow(parent) 
{
    ui = new Ui::PiYingClass();
    ui->setupUi(this);
    setWindowTitle("皮影");
    setFocusPolicy(Qt::StrongFocus);

    float ratio = 16.0f / 9.0f;

    voidListWidget = new QListWidget();
    bgImageList = new QListWidget();
    chImageList = new QListWidget();

    // OpenGL widget
    piYingGL = new PiYingGL(*this);
    piYingGLContainer = new PiYingGLContainer(*piYingGL, ratio, this);

    timeLineGL = new TimeLineGL(this);

    splitTimelineOpenGL = new QSplitter(Qt::Vertical, this);
    splitListOpenGL = new QSplitter(Qt::Horizontal, this);

    ToolButton* chRectSelectVert = new ToolButton(":/PiYing/selectRectChVert_S.png", ":/PiYing/selectRectChVert.png", "selectRectChVert", CharacterToolState::RectSelectVert, this);

    toolChTexList.append(chRectSelectVert);
    toolChTexList.append(new ToolButton(":/PiYing/addChVert_S.png", ":/PiYing/addChVert.png", "addChVert", CharacterToolState::AddTriangle, this));
    toolChTexList.append(new ToolButton(":/PiYing/chAddPoly_S.png", ":/PiYing/chAddPoly.png", "chAddPoly", CharacterToolState::AddPoly, this));
    toolChTexList.append(new ToolButton(":/PiYing/chAddRound_S.png", ":/PiYing/chAddRound.png", "chAddRound", CharacterToolState::AddRound, this));

    for (ToolButton* item : toolChTexList) {
        connect(item->action(), &QAction::triggered, this, [this, item]() {select_tool_texture(item); });
    }

    toolControlSliderList.append(new ToolButton(":/PiYing/addVertTrace_S.png", ":/PiYing/addVertTrace.png", "addVertTrace", CharacterToolState::AddVertTrace, this));

    for (ToolButton* item : toolControlSliderList) {
        connect(item->action(), &QAction::triggered, this, [this, item]() {select_tool_control_slider(item); });
    }

    for (ToolButton* item : toolChSkelenList) {
        connect(item->action(), &QAction::triggered, this, [this, item]() {select_tool_skelen(item); });
    }

    toolChSkelenList.append(chRectSelectVert);


    QComboBox* modeBox = new QComboBox(this);

    // menuBar
    QMenu* menuFile = new QMenu("文件(&F)", this);
    QMenu* menuEdit = new QMenu("编辑(&E)", this);
    ui->menuBar->addMenu(menuFile);
	ui->menuBar->addMenu(menuEdit);

	// child menu of menu File
    QMenu* childMenuImport = menuFile->addMenu("导入");
    QMenu* childMenuExport = menuFile->addMenu("导出");

	// child menu of menu Edit
    QMenu* childMenuScreen = menuEdit->addMenu("幕布");

	// actions of menu File
    QAction* actionExit                 = menuFile->            addAction("退出");
    QAction* actionImportBackGround     = childMenuImport->     addAction("背景图");
    QAction* actionImportCharacter      = childMenuImport->     addAction("角色图");
    QAction* actionExportCurrentFrame   = childMenuExport->     addAction("当前帧");
    QAction* actionExportMainSlider     = childMenuExport->     addAction("主轴");

	// actions of menu Edit
    QAction* actionScreenScale           = childMenuScreen->     addAction("比例...");
    QAction* actionDefaultColor          = childMenuScreen->     addAction("底色...");

    connect(actionExportCurrentFrame,   SIGNAL(triggered()), this, SLOT(exportCurrentFrame()));
    connect(actionExportMainSlider,     SIGNAL(triggered()), this, SLOT(exportMainSlider()));
	connect(actionScreenScale,          SIGNAL(triggered()), this, SLOT(askScreenScale()));
    connect(actionDefaultColor,     &QAction::triggered,                this, [this]() {piYingGL->choseBackgroundColor(); });
    connect(actionExit,             &QAction::triggered,                this, [this]() {close(); });
    connect(actionImportBackGround, &QAction::triggered,                this, [this]() {piYingGL->importBackground(); });
    connect(actionImportCharacter,  &QAction::triggered,                this, [this]() {piYingGL->importChatacter(); });
    connect(chImageList,            &QListWidget::currentItemChanged,   this, [this]() {piYingGL->updateChTool(); piYingGL->update_ch_verts(); });
    connect(bgImageList,            &QListWidget::currentItemChanged,   this, [this]() {piYingGL->update(); });

    piYingGLContainer->setRatio(ratio);
    piYingGL->changeRatio(ratio);

    splitTimelineOpenGL->addWidget(piYingGLContainer);
    splitTimelineOpenGL->addWidget(timeLineGL);
    splitListOpenGL->addWidget(voidListWidget);
    splitListOpenGL->addWidget(splitTimelineOpenGL);
    splitTimelineOpenGL->setSizes({ width() * 5 / 6, width() / 6 });
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
    timeLineGL->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    voidListWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    modeBox->addItems({ "预览模式", "背景编辑", "角色纹理编辑", "角色骨骼编辑", "控制器"});
    ui->statusBar->addWidget(modeBox);

    connect(modeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PiYing::onModeChanged);

    onModeChanged(0);

    setCentralWidget(splitListOpenGL);
}

PiYing::~PiYing()
{
    delete ui;

    auto safeDelete = [](QObject* obj) { if (obj && !obj->parent()) delete obj; };
    safeDelete(voidListWidget);
    safeDelete(bgImageList);
    safeDelete(chImageList);
    safeDelete(piYingGL);
    safeDelete(piYingGLContainer);
    safeDelete(splitTimelineOpenGL);
    safeDelete(splitListOpenGL);

    for (CtrlSlideWidget* item : sliderWidget) safeDelete(item);
}

int PiYing::getCurrentBgRow()
{
    return bgImageList->currentRow();
}

int PiYing::getCurrentChRow()
{
    return chImageList->currentRow();
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

void PiYing::exportMainSlider()
{
    QMessageBox::information(this, "提示", "功能尚未实现");
}

void PiYing::askScreenScale() {
    RatioDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) return;
    Ratio r = dlg.ratio();

    if (r.w == 0 || r.h == 0) return;

    float ratio = float(r.w) / float(r.h);
    piYingGLContainer->setRatio(ratio);
    piYingGLContainer->update();
    piYingGL->changeRatio(ratio);
}
