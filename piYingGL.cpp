#include "piYingGL.h"
#include "piYing.h"
#include "AddTriangle.h"
#include "ChElementSelect.h"
#include "AddChTexPoly.h"

#include <QColorDialog>

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
	characterTriangleIndices.push_back(std::vector<unsigned int>());
	characterVerts.push_back(std::vector<float>());

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

void PiYingGL::setChToolState(ChTexToolState state)
{
	if (chToolState == state) return;
	chToolState = state;

	if (chElementTool) {
		delete chElementTool;
		chElementTool = nullptr;
	}
	
	if (state == ChTexToolState::None) return;
	else if (state == ChTexToolState::AddTriangle) chElementTool = new AddTriangle(characterTriangleIndices, characterVerts);
	else if (state == ChTexToolState::RectSelectVert) chElementTool = new ChElementrrRectSelect(characterTriangleIndices, characterVerts);
	else if (state == ChTexToolState::AddPoly) chElementTool = new AddChTexPoly(characterTriangleIndices, characterVerts);

	update();
}

void PiYingGL::deleteChElement()
{
	int currentVector = parent->chImageList->currentRow();
	if (currentVector < 0) return;
	if (chElementTool) chElementTool->deleteElement(currentVector);
	update();
}

void PiYingGL::enterChElement()
{
	int currentVector = parent->chImageList->currentRow();
	if (currentVector < 0) return;
	if (chElementTool) chElementTool->enter(currentVector);
	update();
}

void PiYingGL::escapeChVert()
{
	if (chElementTool) chElementTool->escape();
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