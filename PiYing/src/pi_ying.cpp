#include "PiYing.h"

#include "ctrlSlideWidget.h"
#include "RatioDialog.h"
#include "piYingGL.h"
#include "piYingGLContainer.h"
#include "tool_button.h"
#include "time_line_gl.h"
#include "cus_func_string.h"
#include "cus_readfile.h"
#include "ch_texture_toolbar.h"
#include "ch_skelen_toolbar.h"
#include "control_slider_toolbar.h"

#include "enum_edit_mode.h"
#include "enum_character_texture_tool_state.h"

#include "ui_PiYing.h"

#include <QMessageBox>
#include <qkeyevent>
#include <qfiledialog.h>
#include <QComboBox>
#include <qlistwidget>
#include <qsplitter>

PiYing* PiYing::_instance = nullptr;

PiYing::PiYing() :
    QMainWindow(nullptr),
    ui(std::make_unique<Ui::PiYingClass>()),
    sliderWidget(new CtrlSlideWidget()),
    voidListWidget(new QListWidget()),
    bgImageList(new QListWidget()),
    chImageList(new QListWidget()),
    splitTimelineOpenGL(new QSplitter(Qt::Vertical, this)),
    splitListOpenGL(new QSplitter(Qt::Horizontal, this)),
    toolChTexList(new ChTextureToolbar()),
    toolChSkelenList(new ChSkelenToolbar()),
    toolControlSliderList(new ControlSliderToolbar())
{
    _instance = this; ///< 单例初始化

    ui->setupUi(this);

    setWindowTitle("皮影");
    setFocusPolicy(Qt::StrongFocus);

    sliderWidget->setStyleSheet(piying::readAllFileToQString(":/PiYing/slideStyle.qss"));
    sliderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // OpenGL widget
    piYingGLContainer = new PiYingGLContainer(PiYingGL::getInstance(), _piying_gl_bg_ratio, this);

    // menuBar
    QMenu* menuFile = new QMenu("文件(&F)", this);
    QMenu* menuEdit = new QMenu("编辑(&E)", this);
    QMenu* menuWindow = new QMenu("窗口(&W)", this);

    ui->menuBar->addMenu(menuFile);
    ui->menuBar->addMenu(menuEdit);
    ui->menuBar->addMenu(menuWindow);

    // child menu of menu File
    QMenu* childMenuImport = menuFile->addMenu("导入");
    QMenu* childMenuExport = menuFile->addMenu("导出");

    // child menu of menu Edit
    QMenu* childMenuScreen = menuEdit->addMenu("幕布");

    // actions of menu Window
    QMenu* childMenuTool = menuWindow->addMenu("打开");

    // actions of menu File
    QAction* actionExit = menuFile->addAction("退出");
    QAction* actionImportBackGround = childMenuImport->addAction("背景图");
    QAction* actionImportCharacter = childMenuImport->addAction("角色图");
    QAction* actionExportCurrentFrame = childMenuExport->addAction("当前帧");
    QAction* actionExportMainSlider = childMenuExport->addAction("主轴");

    // actions of menu Edit
    QAction* actionScreenScale = childMenuScreen->addAction("比例...");
    QAction* actionDefaultColor = childMenuScreen->addAction("底色...");

    // actions of menu Window
    QAction* actionChTool = childMenuTool->addAction("零件库");

    connect(actionExportCurrentFrame, &QAction::triggered, this, [this] { exportCurrentFrame(); });
    connect(actionExportMainSlider, &QAction::triggered, this, [this] { exportMainSlider(); });
    connect(actionScreenScale, &QAction::triggered, this, [this] { askScreenScale(); });
    connect(actionDefaultColor, &QAction::triggered, this, [this] { PiYingGL::getInstance().choseBackgroundColor(); });
    connect(actionExit, &QAction::triggered, this, [this] { close(); });
    connect(actionImportBackGround, &QAction::triggered, this, [this] { PiYingGL::getInstance().importBackground(); });
    connect(actionImportCharacter, &QAction::triggered, this, [this] { PiYingGL::getInstance().importChatacter(); });
    connect(chImageList, &QListWidget::currentItemChanged, this, [this] { PiYingGL::getInstance().update_ch_tool(); PiYingGL::getInstance().update_ch_verts(); });
    connect(bgImageList, &QListWidget::currentItemChanged, this, [this] { PiYingGL::getInstance().update(); });

    piYingGLContainer->setRatio(_piying_gl_bg_ratio);
    PiYingGL::getInstance().changeRatio(_piying_gl_bg_ratio);

    splitTimelineOpenGL->addWidget(piYingGLContainer);
    splitTimelineOpenGL->addWidget(&TimelineGl::getInstance());
    splitListOpenGL->addWidget(voidListWidget);
    splitListOpenGL->addWidget(splitTimelineOpenGL);
    splitTimelineOpenGL->setSizes({ width() * 5 / 6, width() / 6 });
    splitListOpenGL->setStretchFactor(0, 0);
    splitListOpenGL->setStretchFactor(1, 1);

    bgImageList->setUniformItemSizes(true);
    chImageList->setUniformItemSizes(true);
    bgImageList->setIconSize(QSize(50, 50));
    chImageList->setIconSize(QSize(50, 50));

    QString listStyleSheet = piying::readAllFileToQString(":/PiYing/imageListStyle.qss");
    bgImageList->setStyleSheet(listStyleSheet);
    chImageList->setStyleSheet(listStyleSheet);

    PiYingGL::getInstance().setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    TimelineGl::getInstance().setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    voidListWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    bgImageList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    chImageList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QComboBox* modeBox = new QComboBox(this);

    modeBox->addItems(
        {
            "预览模式",
            "背景编辑",
            "角色纹理编辑",
            "初始变形",
            "部件变形"
        }
    );
    ui->statusBar->addWidget(modeBox);

    connect(modeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PiYing::onModeChanged);

    onModeChanged(0);

    setCentralWidget(splitListOpenGL);
}

PiYing::~PiYing()
{
}

PiYing& PiYing::getInstance() noexcept
{
    return *_instance;
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
    switch (event->key()) {
    case Qt::Key_Escape: {
        if (getCurrentChRow() < 0) return;
        PiYingGL::getInstance().escapeChVert();
    }break;
    case Qt::Key_Delete: {
        if (getCurrentChRow() < 0) return;
        PiYingGL::getInstance().deleteChElement();
    }break;
    case Qt::Key_Enter:
    case Qt::Key_Return: {
        if (getCurrentChRow() < 0) return;
        PiYingGL::getInstance().enterChElement();
    }break;
    case Qt::Key_1: {
        switch (PiYingGL::getInstance().editMode) {
        case EditMode::characterTexture: {
            toolChTexList->press_1();
        }break;
        case EditMode::characterSkeleton: {
            toolChSkelenList->press_1();
        }break;
        case EditMode::controlSlide: {
            toolControlSliderList->press_1();
        }break;
        }
    }break;
    case Qt::Key_3: {
        switch (PiYingGL::getInstance().editMode) {
        case EditMode::characterTexture: {
            toolChTexList->press_3();
        }break;
        case EditMode::characterSkeleton: {
            toolChSkelenList->press_3();
        }break;
        case EditMode::controlSlide: {
            toolControlSliderList->press_3();
        }break;
        }
    }break;
    case Qt::Key_4: {
        switch (PiYingGL::getInstance().editMode) {
        case EditMode::controlSlide: {
            toolControlSliderList->press_4();
        }
        }
    }break;
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

    QImage img = PiYingGL::getInstance().grabFramebuffer();

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

    _piying_gl_bg_ratio = static_cast<double>(r.w) / static_cast<double>(r.h);

    if (PiYingGL::getInstance().editMode != EditMode::OverView && PiYingGL::getInstance().editMode != EditMode::BackGround) return;

    piYingGLContainer->setRatio(_piying_gl_bg_ratio);
    piYingGLContainer->update();
    PiYingGL::getInstance().changeRatio(_piying_gl_bg_ratio);
}

void PiYing::set_piying_gl_ratio(double ratio)
{
    piYingGLContainer->setRatio(ratio);
    piYingGLContainer->update();
}

void PiYing::add_bg_item(QListWidgetItem* item)
{
    bgImageList->addItem(item);

    if (getCurrentBgRow() < 0) bgImageList->setCurrentRow(0);
}

void PiYing::add_ch_item(QListWidgetItem* item)
{
    chImageList->addItem(item);

    if (getCurrentChRow() < 0) chImageList->setCurrentRow(0);
}

void PiYing::delete_current_bg_item()
{
    delete bgImageList->takeItem(getCurrentBgRow());
}

void PiYing::delete_all_bg_item()
{
    for (int i = bgImageList->count() - 1; i >= 0; i--) {
        delete bgImageList->takeItem(i);
    }
}

QString PiYing::get_unique_bg_list_name() const
{
    return piying::getUniqueListName(bgImageList);
}

QString PiYing::get_unique_ch_list_name() const
{
    return piying::getUniqueListName(chImageList);
}
