#pragma once

#include <QtWidgets/QMainWindow>
#include <memory>
#include <vector>

enum class CharacterToolState;

class CtrlSlideWidget;
class TimelineGl;
class ToolButton;
class QString;
class PiYingGLContainer;
class QListWidgetItem;
class QListWidget;
class QSplitter;

namespace Ui{
	class PiYingClass;
}

class PiYing : public QMainWindow
{
	Q_OBJECT

private:

public:
	PiYing();
	~PiYing();
	PiYing(const PiYing&) = delete;
	PiYing& operator=(const PiYing&) = delete;

	static PiYing& getInstance();

public:
	int getCurrentBgRow();
	int getCurrentChRow();

	void change_edit_mode_overview();
	void change_edit_mode_character_texture();
	void change_edit_mode_character_skeleton();
	void change_edit_mode_background();
	void change_edit_mode_character_constrol_slider();

	void update_part_slider();
	void set_piying_gl_ratio(double ratio);
	void add_bg_item(QListWidgetItem* item);
	void add_ch_item(QListWidgetItem* item);
	void delete_current_bg_item();
	void delete_all_bg_item();

	QString get_unique_bg_list_name() const;
	QString get_unique_ch_list_name() const;

protected:
	void keyPressEvent(QKeyEvent* event) override;

private slots:
	void exportCurrentFrame();
	void exportMainSlider();
	void askScreenScale();
	void onModeChanged(int mode);

private:
	void select_tool_texture(ToolButton* toolButton);
	void select_tool_skelen(ToolButton* toolButton);
	void select_tool_control_slider(ToolButton* toolButton);
	void adapt_select_tool_button(CharacterToolState state);
	void all_button_unselect();

private:
	std::unique_ptr<Ui::PiYingClass> ui;
	static PiYing* _instance;

	QSplitter* splitListOpenGL;
	QSplitter* splitTimelineOpenGL;

	std::vector<ToolButton*> toolChTexList;
	std::vector<ToolButton*> toolChSkelenList;
	std::vector<ToolButton*> toolControlSliderList;

	ToolButton* _select_button;

	QListWidget* voidListWidget;
	QListWidget* bgImageList = nullptr;
	QListWidget* chImageList = nullptr;

	CtrlSlideWidget* sliderWidget = nullptr;

	PiYingGLContainer* piYingGLContainer = nullptr;

	static QString SLIDER_WIDGET_STYLE_SHEET;
};