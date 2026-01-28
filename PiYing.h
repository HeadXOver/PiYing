#pragma once

#include <QtWidgets/QMainWindow>
#include <memory>

enum class CharacterToolState;
class CtrlSlideWidget;
class TimelineGl;
class ToolButton;
class PiYingGLContainer;
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

	// OpenGL widget
	TimelineGl* timeLineGL = nullptr;

	QSplitter* splitListOpenGL;
	QSplitter* splitTimelineOpenGL;

	QList<ToolButton*> toolChTexList;
	QList<ToolButton*> toolChSkelenList;
	QList<ToolButton*> toolControlSliderList;
	ToolButton* _select_button;
	QListWidget* voidListWidget;

	CtrlSlideWidget* sliderWidget = nullptr;

public:
	static QString SLIDER_WIDGET_STYLE_SHEET;

	QListWidget* bgImageList = nullptr;
	QListWidget* chImageList = nullptr;

	PiYingGLContainer* piYingGLContainer = nullptr;
};