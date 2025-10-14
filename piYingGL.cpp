#include "piYingGL.h"
#include "piYing.h"
#include "AddTriangle.h"
#include "AddChTexPoly.h"
#include "ChElementLibreSelect.h"
#include "CusFunc.h"

#include <QColorDialog>
#include <QMessageBox>
#include <qfiledialog>

PiYingGL::~PiYingGL()
{
	makeCurrent();

	////////////////////////////////////////

	glDeleteBuffers(1, &bgVBO);
	glDeleteVertexArrays(1, &bgVAO);
	glDeleteBuffers(1, &bgEBO);

	////////////////////////////////////////

	doneCurrent();

	if (chElementTool) delete chElementTool;
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

	if (parent->getCurrentBgRow() < 0) parent->bgImageList->setCurrentRow(0);
}

void PiYingGL::addCharacter(const QString& imageName)
{
	QImage img;
	if (!img.load(imageName)) {
		QMessageBox::warning(this, "Warning", "Failed to load image: " + imageName);
		return;
	}
	characterTextures.append(img);
	characterTriangleIndices.push_back(std::vector<unsigned int>());
	characterVerts.push_back(std::vector<float>());
	characterVertsUV.push_back(QList<QPointF>());

	QIcon icon(QPixmap::fromImage(img).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	QListWidgetItem* item = new QListWidgetItem(icon, getUniquebgName(parent->chImageList));
	item->setTextAlignment(Qt::AlignCenter);

	parent->chImageList->addItem(item);

	if (getCurrentChRow() < 0) parent->chImageList->setCurrentRow(0);

	update();
}

void PiYingGL::setEditMode(EditMode mode)
{
	editMode = mode;
	update();
}

void PiYingGL::updateChTexTool()
{
	if (chElementTool) {
		delete chElementTool;
		chElementTool = nullptr;
	}

	int currentVector = getCurrentChRow();
	if (currentVector < 0) return;

	if (chToolState == ChTexToolState::None) return;
	else if (chToolState == ChTexToolState::AddTriangle) chElementTool = new AddTriangle(currentVector, this);
	else if (chToolState == ChTexToolState::RectSelectVert) chElementTool = new ChElementRectSelect(currentVector, this);
	else if (chToolState == ChTexToolState::LibreSelectVert) chElementTool = new ChElementLibreSelect(currentVector, this);
	else if (chToolState == ChTexToolState::AddPoly) chElementTool = new AddChTexPoly(currentVector, this);

	update();
}

void PiYingGL::setChToolState(ChTexToolState state)
{
	chToolState = state;

	if (chElementTool) {
		delete chElementTool;
		chElementTool = nullptr;
	}

	int currentVector = getCurrentChRow();
	if (currentVector < 0) return;
	
	if (state == ChTexToolState::None) return;
	else if (state == ChTexToolState::AddTriangle) chElementTool = new AddTriangle(currentVector, this);
	else if (state == ChTexToolState::RectSelectVert) chElementTool = new ChElementRectSelect(currentVector, this);
	else if (state == ChTexToolState::LibreSelectVert) chElementTool = new ChElementLibreSelect(currentVector, this);
	else if (state == ChTexToolState::AddPoly) chElementTool = new AddChTexPoly(currentVector, this);

	update();
}

void PiYingGL::deleteChElement()
{
	if (chElementTool) chElementTool->deleteElement();
	update();
}

void PiYingGL::enterChElement()
{
	if (chElementTool) chElementTool->enter();
	update();
}

void PiYingGL::escapeChVert()
{
	if (chElementTool) chElementTool->escape();
	update();
}

void PiYingGL::choseBackgroundColor()
{
	QColor color = QColorDialog::getColor(Qt::black , this, "选择幕布底色");
	if (!color.isValid()) return;
	makeCurrent();
	glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
	glClear(GL_COLOR_BUFFER_BIT);
	doneCurrent();
	update();
}

void PiYingGL::changeRatio(float ratio)
{
	if (ratio > 1.0f) {
		proj.setToIdentity();
		proj.ortho(-ratio, ratio, -1, 1, -1, 1);
		insProj.setToIdentity();
		insProj.ortho(-1.0f / ratio, 1.0f / ratio, -1, 1, -1, 1);
	}
	else {
		proj.setToIdentity();
		proj.ortho(-1, 1, -1.0f / ratio, 1.0f / ratio, -1, 1);
		insProj.setToIdentity();
		insProj.ortho(-1, 1, ratio, ratio, -1, 1);
	}
	update();
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
	if (pos == MousePos::OutSide) return Qt::ArrowCursor;
	if (pos == MousePos::Inside) return Qt::OpenHandCursor;
	if (!KeyboardStateWin::isAltHeld()) return Qt::CursorShape::CrossCursor;
	return Qt::OpenHandCursor;
}

int PiYingGL::getCurrentBgRow() const
{
	return parent->getCurrentBgRow();
}

int PiYingGL::getCurrentChRow() const
{
	return parent->getCurrentChRow();
}
