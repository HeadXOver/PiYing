#pragma once

#include <QtWidgets/QMainWindow>
#include <memory>

class CtrlSlideWidget;
class PiYingGL;
class TimelineGl;
class ToolButton;
class PiYingGLContainer;
class QListWidget;
class QSplitter;
class QSlider;
class SlideApplier;

namespace Ui{
	class PiYingClass;
}

class PiYing : public QMainWindow
{
    Q_OBJECT

public:
    PiYing(QWidget *parent = nullptr);
    ~PiYing();

public:
	int getCurrentBgRow();
	int getCurrentChRow();

	void change_edit_mode_overview();
	void change_edit_mode_character_texture();
	void change_edit_mode_character_skeleton();
	void change_edit_mode_background();
	void change_edit_mode_character_constrol_slider();

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

private:
    std::unique_ptr<Ui::PiYingClass> ui;

	// OpenGL widget
    PiYingGL* piYingGL = nullptr;
	TimelineGl* timeLineGL = nullptr;

	QSplitter* splitListOpenGL;
	QSplitter* splitTimelineOpenGL;

	QList<ToolButton*> toolChTexList;
	QList<ToolButton*> toolChSkelenList;
	QList<ToolButton*> toolControlSliderList;

public:
	static QString SLIDER_WIDGET_STYLE_SHEET;

	PiYingGLContainer* piYingGLContainer = nullptr;

	QListWidget* bgImageList = nullptr;
	QListWidget* chImageList = nullptr;
	QListWidget* voidListWidget;

	QList<CtrlSlideWidget*> sliderWidget;
};

