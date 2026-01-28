#include "PiYing.h"

#include "ctrlSlideWidget.h"
#include "RatioDialog.h"
#include "piYingGL.h"
#include "piYingGLContainer.h"
#include "tool_button.h"
#include "time_line_gl.h"

#include "enum_edit_mode.h"
#include "enum_character_texture_tool_state.h"

#include "ui_PiYing.h"

#include <QMessageBox>
#include <qkeyevent>
#include <qfiledialog.h>
#include <QComboBox>
#include <qlistwidget>
#include <qsplitter>

QString PiYing::SLIDER_WIDGET_STYLE_SHEET;
PiYing* PiYing::_instance = nullptr;

PiYing::PiYing() : QMainWindow(nullptr)
{
    _instance = this;

    QFile sliderStyle(":/PiYing/slideStyle.qss");
    if (sliderStyle.open(QFile::ReadOnly)) {
        SLIDER_WIDGET_STYLE_SHEET = sliderStyle.readAll();
        sliderStyle.close();
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to load style sheet");
    }

    ui = std::make_unique<Ui::PiYingClass>();
    ui->setupUi(this);
    setWindowTitle("皮影");
    setFocusPolicy(Qt::StrongFocus);

    sliderWidget = new CtrlSlideWidget();
    sliderWidget->setStyleSheet(SLIDER_WIDGET_STYLE_SHEET);
    sliderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    float ratio = 16.0f / 9.0f;

    voidListWidget = new QListWidget();
    bgImageList = new QListWidget();
    chImageList = new QListWidget();

    // OpenGL widget
    piYingGLContainer = new PiYingGLContainer(PiYingGL::getInstance(), ratio, this);

    splitTimelineOpenGL = new QSplitter(Qt::Vertical, this);
    splitListOpenGL = new QSplitter(Qt::Horizontal, this);

    _select_button = new ToolButton(":/PiYing/selectRectChVert_S.png", ":/PiYing/selectRectChVert.png", "selectRectChVert", CharacterToolState::RectSelectVert, this);

    toolChTexList.append(_select_button);
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

    toolChSkelenList.append(_select_button); ///< 在连接之后添加，因为chRectSelectVert在toolChSkelenList中

    QComboBox* modeBox = new QComboBox(this);

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
    QAction* actionExit                 = menuFile->            addAction("退出");
    QAction* actionImportBackGround     = childMenuImport->     addAction("背景图");
    QAction* actionImportCharacter      = childMenuImport->     addAction("角色图");
    QAction* actionExportCurrentFrame   = childMenuExport->     addAction("当前帧");
    QAction* actionExportMainSlider     = childMenuExport->     addAction("主轴");

	// actions of menu Edit
    QAction* actionScreenScale           = childMenuScreen->     addAction("比例...");
    QAction* actionDefaultColor          = childMenuScreen->     addAction("底色...");

    // actions of menu Window
    QAction* actionChTool = childMenuTool->addAction("零件库");

    connect(actionExportCurrentFrame,   SIGNAL(triggered()), this, SLOT(exportCurrentFrame()));
    connect(actionExportMainSlider,     SIGNAL(triggered()), this, SLOT(exportMainSlider()));
	connect(actionScreenScale,          SIGNAL(triggered()), this, SLOT(askScreenScale()));
    connect(actionDefaultColor,     &QAction::triggered,                this, [this]() {PiYingGL::getInstance().choseBackgroundColor(); });
    connect(actionExit,             &QAction::triggered,                this, [this]() {close(); });
    connect(actionImportBackGround, &QAction::triggered,                this, [this]() {PiYingGL::getInstance().importBackground(); });
    connect(actionImportCharacter,  &QAction::triggered,                this, [this]() {PiYingGL::getInstance().importChatacter(); });
    connect(chImageList,            &QListWidget::currentItemChanged,   this, [this]() {PiYingGL::getInstance().update_ch_tool(); PiYingGL::getInstance().update_ch_verts(); });
    connect(bgImageList,            &QListWidget::currentItemChanged,   this, [this]() {PiYingGL::getInstance().update(); });

    piYingGLContainer->setRatio(ratio);
    PiYingGL::getInstance().changeRatio(ratio);

    splitTimelineOpenGL->addWidget(piYingGLContainer);
    splitTimelineOpenGL->addWidget(&TimelineGl::getInstance());
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

    PiYingGL::getInstance().setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    TimelineGl::getInstance().setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    voidListWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    modeBox->addItems(
        {
            "预览模式",
            "背景编辑",
            "角色纹理编辑",
            "角色骨骼编辑",
            "拼合台"
        }
    );
    ui->statusBar->addWidget(modeBox);

    connect(modeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PiYing::onModeChanged);

    onModeChanged(0);

    setCentralWidget(splitListOpenGL);
}

PiYing::~PiYing()
{
    auto safeDelete = [](QObject* obj) { if (obj && !obj->parent()) delete obj; };
    safeDelete(voidListWidget);
    safeDelete(bgImageList);
    safeDelete(chImageList);
    safeDelete(&PiYingGL::getInstance());
    safeDelete(piYingGLContainer);
    safeDelete(splitTimelineOpenGL);
    safeDelete(splitListOpenGL);
    safeDelete(&TimelineGl::getInstance());
    safeDelete(sliderWidget);
}

PiYing& PiYing::getInstance()
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
        switch (PiYingGL::getInstance().ch_tool_state()) {
        case CharacterToolState::RectSelectTriangle: {
            adapt_select_tool_button(CharacterToolState::RectSelectVert);
            PiYingGL::getInstance().setChTool(CharacterToolState::RectSelectVert);
        }break;
        case CharacterToolState::LibreSelectTriangle: {
            adapt_select_tool_button(CharacterToolState::LibreSelectVert);
            PiYingGL::getInstance().setChTool(CharacterToolState::LibreSelectVert);
        }break;
        case CharacterToolState::LibreSelectVert:
        case CharacterToolState::RectSelectVert: return;
        default: {
            all_button_unselect();

            if (_select_button->toolState() == CharacterToolState::RectSelectTriangle) {
                adapt_select_tool_button(CharacterToolState::RectSelectVert);
            }
            else if (_select_button->toolState() == CharacterToolState::LibreSelectTriangle) {
                adapt_select_tool_button(CharacterToolState::LibreSelectVert);
            }

            _select_button->select();
            PiYingGL::getInstance().setChTool(_select_button->toolState());
        }
        }
    }break;
    case Qt::Key_3: {
        switch (PiYingGL::getInstance().ch_tool_state()) {
        case CharacterToolState::RectSelectVert: {
            adapt_select_tool_button(CharacterToolState::RectSelectTriangle);
            PiYingGL::getInstance().setChTool(CharacterToolState::RectSelectTriangle);
        }break;
        case CharacterToolState::LibreSelectVert: {
            adapt_select_tool_button(CharacterToolState::LibreSelectTriangle);
            PiYingGL::getInstance().setChTool(CharacterToolState::LibreSelectTriangle);
        }break;
        case CharacterToolState::LibreSelectTriangle:
        case CharacterToolState::RectSelectTriangle: return;
        default: {
            all_button_unselect();

            if (_select_button->toolState() == CharacterToolState::RectSelectVert) {
                adapt_select_tool_button(CharacterToolState::RectSelectTriangle);
            }
            else if (_select_button->toolState() == CharacterToolState::LibreSelectVert) {
                adapt_select_tool_button(CharacterToolState::LibreSelectTriangle);
            }

            _select_button->select();
            PiYingGL::getInstance().setChTool(_select_button->toolState());
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

    float ratio = float(r.w) / float(r.h);
    piYingGLContainer->setRatio(ratio);
    piYingGLContainer->update();
    PiYingGL::getInstance().changeRatio(ratio);
}