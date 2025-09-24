#include "piYingGL.h"
#include "piYing.h"

#include <QColorDialog>

PiYingGL::PiYingGL(PiYing* parent) : QOpenGLWidget(parent), parent(parent)
{
	setAutoFillBackground(false);

	proj.setToIdentity();
	insProj.setToIdentity();
	viewScale.setValue(1.0f);

	aspect = 16.0f / 9.0f;

	actionAddBackGround = new QAction("添加背景图", this);
	actionAddCharacterTexture = new QAction("添加角色图", this);
	actionFullScreenBackGround = new QAction("背景图全屏", this);
	actionChoseBackGroundColor = new QAction("选择幕布底色", this);
	actionSetViewToStandard = new QAction("将视图设为标准幕布", this);
	actionReturnToStandard = new QAction("返回标准幕布视图", this);
	actionDeleteBg = new QAction("删除当前背景图");
	actionDeleteAllBg = new QAction("删除所有背景图");
	actionBgSetTransform = new QAction("设置变换...");
	actionAgainstBg = new QAction("将摄像机对准图片");
	actionReturnbgTransform = new QAction("还原变换");

	labelViewScale = new QLabel("1", this);
	labelViewTransX = new QLabel("0", this);
	labelViewTransY = new QLabel("0", this);
	labelViewRot = new QLabel("0", this);

	connect(actionFullScreenBackGround, SIGNAL(triggered()), this, SLOT(fullScreenBackGround()));
	connect(actionSetViewToStandard,	SIGNAL(triggered()), this, SLOT(setViewToStandard()));
	connect(actionReturnToStandard,		SIGNAL(triggered()), this, SLOT(returnToStandard()));
	connect(actionDeleteBg,				SIGNAL(triggered()), this, SLOT(deleteBg()));
	connect(actionDeleteAllBg,			SIGNAL(triggered()), this, SLOT(deleteAllBg()));
	connect(actionAgainstBg,			SIGNAL(triggered()), this, SLOT(againstBg()));
	connect(actionBgSetTransform,		SIGNAL(triggered()), this, SLOT(bgSetTransform()));
	connect(actionReturnbgTransform,	SIGNAL(triggered()), this, SLOT(returnbgTransform()));
	connect(actionAddCharacterTexture,	&QAction::triggered, this, [this]() {importChatacter(); });
	connect(actionAddBackGround,		&QAction::triggered, this, [this]() {importBackground(); });
	connect(actionChoseBackGroundColor, &QAction::triggered, this, [this]() {choseBackgroundColor(); });
	connect(&viewScale,		&ViewData::valueChanged, this, [&](float v) {labelViewScale->setText(tr("%1  ").arg(v)); });
	connect(&viewTransX,	&ViewData::valueChanged, this, [&](float v) {labelViewTransX->setText(tr("%1  ").arg(v)); });
	connect(&viewTransY,	&ViewData::valueChanged, this, [&](float v) {labelViewTransY->setText(tr("%1  ").arg(v)); });
	connect(&viewRotate,	&ViewData::valueChanged, this, [&](float v) {labelViewRot->setText(tr("%1  ").arg(v)); });

	setMouseTracking(true);

	backGroundColor.setRgb(0, 0, 0);
}

PiYingGL::~PiYingGL()
{
	makeCurrent();

	////////////////////////////////////////

	glDeleteBuffers(1, &bgVBO);
	glDeleteVertexArrays(1, &bgVAO);
	glDeleteBuffers(1, &bgEBO);

	////////////////////////////////////////

	doneCurrent();
}

void PiYingGL::addBackground(const QString& imageName) {
	QImage img;
	if (!img.load(imageName)) {
		QMessageBox::warning(this, "Warning", "Failed to load image: " + imageName);
		return;
	}
	appendBgList(img);

	update();
}

bool PiYingGL::addBackground(const QString& imageName, QImage& image)
{
	QImage img;
	if (!img.load(imageName)) {
		QMessageBox::warning(this, "Warning", "Failed to load image: " + imageName);
		return false;
	}
	appendBgList(img);
	image = img;

	update();
	return true;
}

void PiYingGL::appendBgList(QImage& image)
{
	backGrounds.append(image);

	QIcon icon(QPixmap::fromImage(image).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	QListWidgetItem* item = new QListWidgetItem(icon, getUniquebgName(parent->bgImageList));
	item->setTextAlignment(Qt::AlignCenter);

	parent->bgImageList->addItem(item);

	if (parent->bgImageList->currentRow() < 0) parent->bgImageList->setCurrentRow(0);
}

void PiYingGL::addCharacter(const QString& imageName)
{
	QImage img;
	if (!img.load(imageName)) {
		QMessageBox::warning(this, "Warning", "Failed to load image: " + imageName);
		return;
	}
	characterTextures.append(img);

	QIcon icon(QPixmap::fromImage(img).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	QListWidgetItem* item = new QListWidgetItem(icon, getUniquebgName(parent->chImageList));
	item->setTextAlignment(Qt::AlignCenter);

	parent->chImageList->addItem(item);

	if (parent->chImageList->currentRow() < 0) parent->chImageList->setCurrentRow(0);

	update();
}

void PiYingGL::setEditMode(EditMode mode)
{
	editMode = mode;
	currentSelectedBackGround = -1;
	update();
}

void PiYingGL::choseBackgroundColor()
{
	QColor color = QColorDialog::getColor(backGroundColor, this, "选择幕布底色");
	if (!color.isValid()) return;
	backGroundColor = color;
	makeCurrent();
	glClearColor(backGroundColor.redF(), backGroundColor.greenF(), backGroundColor.blueF(), backGroundColor.alphaF());
	glClear(GL_COLOR_BUFFER_BIT);
	doneCurrent();
	update();
}

void PiYingGL::changeRatio(float ratio)
{
	aspect = ratio;
	updateProjMatrix();
	update();
}

void PiYingGL::updateProjMatrix()
{
	if (aspect > 1.0f) {
		proj.setToIdentity();
		proj.ortho(-aspect, aspect, -1, 1, -1, 1);
		insProj.setToIdentity();
		insProj.ortho(-1.0f / aspect, 1.0f / aspect, -1, 1, -1, 1);
	}
	else {
		proj.setToIdentity();
		proj.ortho(-1, 1, -1.0f / aspect, 1.0f / aspect, -1, 1);
		insProj.setToIdentity();
		insProj.ortho(-1, 1, aspect, aspect, -1, 1);
	}
}

void PiYingGL::importBackground()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, "选择背景图", ".", "Images (*.png *.xpm *.jpg)");

	if (fileNames.size() == 1) {
		QImage img;
		if (!addBackground(fileNames[0], img)) return;

		int ret = QMessageBox::question(
			this,
			tr("提示"),
			tr("是否使用图片比例？"),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::Yes
		);

		if (ret == QMessageBox::Yes) {
			float ratio = img.width() / (float)img.height();
			parent->piYingGLContainer->setRatio(ratio);
			parent->piYingGLContainer->update();
			changeRatio(ratio);
		}

		update();
	}
	else for (const QString& fileName : fileNames) addBackground(fileName);
}

void PiYingGL::importChatacter()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, "选择角色图", ".", "Images (*.png *.xpm *.jpg)");

	for (const QString& fileName : fileNames) addCharacter(fileName);
}

MousePos PiYingGL::getMousePosType(const QPointF& point) const
{
	if (isInsideSquare(point)) {
		if (point.x() < -0.9f) {
			if (point.y() > 0.9f) return MousePos::LeftTop;
			if (point.y() < -0.9f)  return MousePos::LeftBottom;
			return MousePos::LeftEdge;
		}
		if (point.x() > 0.9f) {
			if (point.y() > 0.9f) return MousePos::RightTop;
			if (point.y() < -0.9f)  return MousePos::RightBottom;
			return MousePos::RightEdge;
		}
		if (point.y() > 0.9f) return MousePos::TopEdge;
		if (point.y() < -0.9f) return MousePos::BottomEdge;
		return MousePos::Inside;
	}
	return MousePos::OutSide;
}

Qt::CursorShape PiYingGL::getCursorShape(const MousePos& pos)
{
	switch (pos) {
	case MousePos::LeftTop:
	case MousePos::RightBottom:	//return Qt::SizeFDiagCursor;
	case MousePos::LeftBottom:
	case MousePos::RightTop:	//return Qt::SizeBDiagCursor;
	case MousePos::LeftEdge:
	case MousePos::RightEdge:	//return Qt::SizeHorCursor;
	case MousePos::TopEdge:
	case MousePos::BottomEdge:  if (!KeyboardStateWin::isAltHeld()) return Qt::CursorShape::CrossCursor;
	case MousePos::Inside:		return Qt::OpenHandCursor;
	default:					return Qt::ArrowCursor;
	}
}

QString PiYingGL::getUniquebgName(const QListWidget* list)
{
	QString s;
	bool repeat = false;
	for (int i = 1;; i++) {
		s = QString::number(i);
		repeat = false;
		for (int j = 0; j < list->count(); ++j)
			if (list->item(j)->text() == s) repeat = true;
		if (!repeat) return s;
	}
}