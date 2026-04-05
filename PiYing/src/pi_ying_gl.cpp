#include "piYingGL.h"

#include "piYing.h"
#include "time_line_gl.h"
#include "piying_tool.h"

#include "point_vector.h"
#include "parts.h"
#include "part.h"
#include "piying_texture.h"

#include "cus_func_zone.h"

#include "enum_edit_mode.h"
#include "enum_character_texture_tool_state.h"

#include <QColorDialog>
#include <QMessageBox>
#include <qfiledialog>
#include <qlistwidget>

void PiYingGL::addBackground(const QString& imageName) {
	QImage img;
	if (!img.load(imageName)) {
		QMessageBox::warning(this, "Warning", "Failed to load image: " + imageName);
		return;
	}
	appendBgList(img);

	update();
}

bool PiYingGL::addBackground(const QString& imageName, float& o_ratio)
{
	QImage img;
	if (!img.load(imageName)) {
		QMessageBox::warning(this, "Warning", "Failed to load image: " + imageName);
		return false;
	}

	o_ratio = img.width() / (float)img.height();

	appendBgList(img);

	update();
	return true;
}

void PiYingGL::appendBgList(const QImage& image)
{
	backGrounds.append(new piying::ImageTexture(image, width() / float(height())));

	QIcon icon(QPixmap::fromImage(image).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	QListWidgetItem* item = new QListWidgetItem(icon, PiYing::getInstance().get_unique_bg_list_name());
	item->setTextAlignment(Qt::AlignCenter);

	PiYing::getInstance().add_bg_item(item);
}

void PiYingGL::add_character(const QString& imageName)
{
	QImage img;
	if (!img.load(imageName)) {
		QMessageBox::warning(this, "Warning", "Failed to load image: " + imageName);
		return;
	}
	makeCurrent();
	characterTextures.append(new piying::Texture(img));
	doneCurrent();
	characterTriangleIndices.push_back(std::vector<unsigned int>());
	characterVerts.push_back(new PointVectorLayer());

	QIcon icon(QPixmap::fromImage(img).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));

	QListWidgetItem* item = new QListWidgetItem(icon, PiYing::getInstance().get_unique_ch_list_name());
	item->setTextAlignment(Qt::AlignCenter);

	if (getCurrentChRow() < 0) {
		_currentLayer = characterVerts[0];
		_currentIndex = &characterTriangleIndices[0];
	}

	PiYing::getInstance().add_ch_item(item);

	update();
}

void PiYingGL::setEditMode(EditMode mode)
{
	editMode = mode;
	update_ch_tool();
	update();
}

void PiYingGL::update_ch_tool()
{
	ch_element_tool_ = nullptr;

	switch (editMode) {
	case EditMode::characterSkeleton:
	case EditMode::characterTexture: {
		_currentLayer = nullptr;

		int currentVector = getCurrentChRow();
		if (currentVector < 0) return;

		_currentLayer = characterVerts[currentVector];
		_currentIndex = &characterTriangleIndices[currentVector];
		ch_element_tool_ = std::make_unique<piying::tool::PiyingTool>(_ch_tool_state);
	}break;
	case EditMode::controlSlide: {
		if(!TimelineGl::getInstance().get_current_part()) return;
		if (_ch_tool_state == CharacterToolState::None) {
			ch_element_tool_ = std::make_unique<piying::tool::PiyingTool>(CharacterToolState::PartHandle);
		}
		else{
			ch_element_tool_ = std::make_unique<piying::tool::PiyingTool>(_ch_tool_state);
		}
	}break;
	}

	update();
}

void PiYingGL::setChTool(CharacterToolState state)
{
	_ch_tool_state = state;

	update_ch_tool();
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
		_orth_ratio.setToIdentity();
		_orth_ratio.ortho(-ratio, ratio, -1, 1, -1, 1);
		_orth_ratio_invert.setToIdentity();
		_orth_ratio_invert.ortho(-1.0f / ratio, 1.0f / ratio, -1, 1, -1, 1);
	}
	else {
		_orth_ratio.setToIdentity();
		_orth_ratio.ortho(-1, 1, -1.0f / ratio, 1.0f / ratio, -1, 1);
		_orth_ratio_invert.setToIdentity();
		_orth_ratio_invert.ortho(-1, 1, -ratio, ratio, -1, 1);
	}
	update();
}

void PiYingGL::importBackground()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, "选择背景图", ".", "Images (*.png *.xpm *.jpg)");

	if (fileNames.size() == 1) {
		float ratio;
		if (!addBackground(fileNames[0], ratio)) return;

		int ret = QMessageBox::question(
			this,
			tr("提示"),
			tr("是否使用图片比例？"),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::Yes
		);

		if (ret == QMessageBox::Yes) {
			PiYing::getInstance().set_piying_gl_ratio(ratio);
			changeRatio(ratio);
		}

		update();
	}
	else for (const QString& fileName : fileNames) addBackground(fileName);
}

void PiYingGL::importChatacter()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, "选择角色图", ".", "Images (*.png *.xpm *.jpg)");

	for (const QString& fileName : fileNames) add_character(fileName);
}

MousePos PiYingGL::getMousePosType(const QPointF& point) const noexcept
{
	if (piying::isInsideSquare(point)) {
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

int PiYingGL::getCurrentBgRow() const
{
	return PiYing::getInstance().getCurrentBgRow();
}

int PiYingGL::getCurrentChRow() const
{
	return PiYing::getInstance().getCurrentChRow();
}

void PiYingGL::add_part(const QList<unsigned int>& indices)
{
	const int currentVector = getCurrentChRow();
	if (currentVector < 0) return;

	Parts::getInstance().add(new Part(*characterTextures[currentVector], indices, editMode == EditMode::characterTexture));
	TimelineGl::getInstance().update_showing_parts();
	TimelineGl::getInstance().init_part_cursor();
	TimelineGl::getInstance().update_is_draw_by_piying();
	PiYing::getInstance().update_part_slider();
	update();
	TimelineGl::getInstance().update();
}

PointVectorLayer* PiYingGL::currentLayer() noexcept
{
	switch (editMode) {
	case EditMode::characterTexture:
	case EditMode::characterSkeleton: {
		return _currentLayer;
	}break;
	case EditMode::controlSlide: {
		return TimelineGl::getInstance().currentLayer();
	}break;
	default: {
		return nullptr;
	}
	}
}

float PiYingGL::view_rotate_degree() const noexcept
{
	return viewRotate.value();
}