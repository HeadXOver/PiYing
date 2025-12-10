#include "piYingGL.h"

#include "piYing.h"

#include "add_triangle.h"
#include "add_ch_tex_poly.h"
#include "ch_element_libre_select.h"

#include "ch_element_tool.h"
#include "point_vector.h"
#include "point_vector_layer.h"
#include "vert_groups.h"
#include "part.h"

#include "piYingGLContainer.h"
#include "ctrlSlideWidget.h"

#include "KeyboardStateWin.h"

#include "image_transform.h"
#include "image_texture.h"

#include "cus_func_string.h"

#include "enum_edit_mode.h"

#include "global_objects.h"

#include <QColorDialog>
#include <QMessageBox>
#include <qfiledialog>
#include <qlistwidget>
#include <qopenglshaderprogram>
#include <qmenu>

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

void PiYingGL::appendBgList(const QImage& image)
{
	backGrounds.append(new ImageTexture(image));

	QIcon icon(QPixmap::fromImage(image).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	QListWidgetItem* item = new QListWidgetItem(icon, getUniquebgName(ref_PiYing.bgImageList));
	item->setTextAlignment(Qt::AlignCenter);

	ref_PiYing.bgImageList->addItem(item);

	if (ref_PiYing.getCurrentBgRow() < 0) ref_PiYing.bgImageList->setCurrentRow(0);
}

void PiYingGL::addCharacter(const QString& imageName)
{
	QImage img;
	if (!img.load(imageName)) {
		QMessageBox::warning(this, "Warning", "Failed to load image: " + imageName);
		return;
	}
	makeCurrent();
	characterTextures.append(new ImageTexture(img));
	doneCurrent();
	characterTriangleIndices.push_back(std::vector<unsigned int>());
	characterVerts.push_back(new PointVector());
	_character_vert_groups.push_back(new VertGroups());

	QIcon icon(QPixmap::fromImage(img).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	QListWidgetItem* item = new QListWidgetItem(icon, getUniquebgName(ref_PiYing.chImageList));
	item->setTextAlignment(Qt::AlignCenter);

	ref_PiYing.chImageList->addItem(item);

	CtrlSlideWidget* sliderWidget = new CtrlSlideWidget(*this, "1");
	sliderWidget->setStyleSheet(PiYing::SLIDER_WIDGET_STYLE_SHEET);
	sliderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	ref_PiYing.sliderWidget.append(sliderWidget);

	if (getCurrentChRow() < 0) {
		ref_PiYing.chImageList->setCurrentRow(0);
		currentLayer = new PointVectorLayer(*characterVerts[0]);
	}

	update();
}

void PiYingGL::setEditMode(EditMode mode)
{
	editMode = mode;
	updateChTool();
}

void PiYingGL::updateChTool()
{
	if (ch_element_tool_) {
		ch_element_tool_ = nullptr;
	}

	if (currentLayer) {
		delete currentLayer;
		currentLayer = nullptr;
	}

	int currentVector = getCurrentChRow();
	if (currentVector < 0) return;

	currentLayer = new PointVectorLayer(*characterVerts[currentVector]);
	currentIndex = &characterTriangleIndices[currentVector];

	if (_ch_tool_state == CharacterToolState::None) {
		update();
		return;
	}

	if (editMode == EditMode::characterSkeleton) {
		if (_ch_tool_state == CharacterToolState::RectSelectVert || _ch_tool_state == CharacterToolState::LibreSelectVert) {
			ch_element_tool_ = std::make_unique<ChElementTool>(currentVector, *this, _ch_tool_state);
		}
		update();
		return;
	}

	if (editMode == EditMode::controlSlide) {
		if (_ch_tool_state == CharacterToolState::AddVertTrace) {
			ch_element_tool_ = std::make_unique<ChElementTool>(currentVector, *this, _ch_tool_state);
		}
		update();
		return;
	}

	if (editMode == EditMode::characterTexture) {
		if (_ch_tool_state == CharacterToolState::RectSelectVert 
			|| _ch_tool_state == CharacterToolState::LibreSelectVert
			|| _ch_tool_state == CharacterToolState::AddPoly
			|| _ch_tool_state == CharacterToolState::AddRound
			|| _ch_tool_state == CharacterToolState::AddTriangle) {
			ch_element_tool_ = std::make_unique<ChElementTool>(currentVector, *this, _ch_tool_state);
		}
		update();
		return;
	}

	update();
}

void PiYingGL::setChTool(CharacterToolState state)
{
	_ch_tool_state = state;

	if (ch_element_tool_) {
		ch_element_tool_ = nullptr;
	}

	const int currentVector = getCurrentChRow();
	if (currentVector < 0) return;
	
	if (state == CharacterToolState::None) {
		update();
		return;
	}

	ch_element_tool_ = std::make_unique<ChElementTool>(currentVector, *this, _ch_tool_state);

	update();
}

void PiYingGL::deleteChElement()
{
if (ch_element_tool_) ch_element_tool_->delete_element();
	update();
}

void PiYingGL::enterChElement()
{
	if (ch_element_tool_) ch_element_tool_->enter();
	update();
}

void PiYingGL::escapeChVert()
{
	if (ch_element_tool_) ch_element_tool_->escape();
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
			ref_PiYing.piYingGLContainer->setRatio(ratio);
			ref_PiYing.piYingGLContainer->update();
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

Qt::CursorShape PiYingGL::getCursorShape(const MousePos& pos) const
{
	if (pos == MousePos::OutSide) return Qt::ArrowCursor;
	if (pos == MousePos::Inside) return Qt::OpenHandCursor;
	if (!KeyboardStateWin::isAltHeld()) return Qt::CursorShape::CrossCursor;
	return Qt::OpenHandCursor;
}

int PiYingGL::getCurrentBgRow() const
{
	return ref_PiYing.getCurrentBgRow();
}

int PiYingGL::getCurrentChRow() const
{
	return ref_PiYing.getCurrentChRow();
}

void PiYingGL::addGlobalAction(QMenu* menu, const QList<QAction*> action)
{
	for (QAction* item : action)  menu->addAction(item);
}

void PiYingGL::add_part(const QList<unsigned int>& indices)
{
	const int currentVector = getCurrentChRow();
	if (currentVector < 0) return;

	_parts.push_back(new Part(*characterTextures[currentVector]->texture(), indices, editMode == EditMode::characterTexture));
}

int PiYingGL::get_group_num() const
{
	const int currentVector = getCurrentChRow();
	if (currentVector < 0) return 0;

	return _character_vert_groups[currentVector]->group_num();
}
