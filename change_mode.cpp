#include "PiYing.h"
#include "PiYingGL.h"
#include "tool_button.h"
#include "ctrlSlideWidget.h"
#include "part.h"
#include "time_line_gl.h"
#include "enum_edit_mode.h"

#include "ui_PiYing.h"

#include <qsplitter>
#include <qlistwidget>

namespace {

    constexpr void(*change_edit_mode[])() = {
        []() { PiYing::getInstance().change_edit_mode_overview(); },
        []() { PiYing::getInstance().change_edit_mode_background(); },
        []() { PiYing::getInstance().change_edit_mode_character_texture(); },
        []() { PiYing::getInstance().change_edit_mode_character_skeleton(); },
        []() { PiYing::getInstance().change_edit_mode_character_constrol_slider(); }
    };

    using change_edit_handler = void(*)();
    constexpr change_edit_handler map_change_edit(int state) {
        return change_edit_mode[state];
    }

}

void PiYing::change_edit_mode_overview()
{
    splitListOpenGL->widget(0)->setParent(nullptr);
    splitListOpenGL->insertWidget(0, voidListWidget);

    splitTimelineOpenGL->setSizes({ width() * 5 / 6, width() / 6 });

    TimelineGl::getInstance().set_to_timeline();

    PiYingGL::getInstance().setChTool(CharacterToolState::None);
    PiYingGL::getInstance().setEditMode(EditMode::OverView);
}

void PiYing::change_edit_mode_background()
{
    splitListOpenGL->widget(0)->setParent(nullptr);
    splitListOpenGL->insertWidget(0, bgImageList);

    splitTimelineOpenGL->setSizes({ width() * 5 / 6, width() / 6 });

    TimelineGl::getInstance().set_to_timeline();

    PiYingGL::getInstance().setChTool(CharacterToolState::None);
    PiYingGL::getInstance().setEditMode(EditMode::BackGround);
}

void PiYing::change_edit_mode_character_texture()
{
    splitListOpenGL->widget(0)->setParent(nullptr);
    splitListOpenGL->insertWidget(0, chImageList);

    splitTimelineOpenGL->setSizes({ width() * 3 / 4, width() / 4 });

    TimelineGl::getInstance().set_to_part();

    PiYingGL::getInstance().setEditMode(EditMode::characterTexture);

    for (ToolButton* item : toolChTexList) {
        ui->mainToolBar->addAction(item->action());
    }

    for (ToolButton* item : toolChTexList) {
        if (item->isSelect()) {
            PiYingGL::getInstance().setChTool(item->toolState());
            return;
        }
    }

    if(!PiYingGL::getInstance().have_ch_tool()) PiYingGL::getInstance().setChTool(CharacterToolState::None);
}

void PiYing::change_edit_mode_character_skeleton()
{
    splitListOpenGL->widget(0)->setParent(nullptr);
    splitListOpenGL->insertWidget(0, chImageList);

    splitTimelineOpenGL->setSizes({ width() * 3 / 4, width() / 4 });

    TimelineGl::getInstance().set_to_part();

    PiYingGL::getInstance().setEditMode(EditMode::characterSkeleton);

    for (ToolButton* item : toolChSkelenList) {
        ui->mainToolBar->addAction(item->action());
    }

    for (ToolButton* item : toolChSkelenList) {
        if (item->isSelect()) {
            PiYingGL::getInstance().setChTool(item->toolState());
            return;
        }
    }

    if (!PiYingGL::getInstance().have_ch_tool()) PiYingGL::getInstance().setChTool(CharacterToolState::None);
}

void PiYing::change_edit_mode_character_constrol_slider()
{
    splitListOpenGL->widget(0)->setParent(nullptr);
    Part* part = TimelineGl::getInstance().get_current_part();
    if(part)
        splitListOpenGL->insertWidget(0, sliderWidget);
    else
        splitListOpenGL->insertWidget(0, voidListWidget);
    PiYingGL::getInstance().setEditMode(EditMode::controlSlide);
    splitListOpenGL->setSizes({ width() / 5, width() * 4 / 5 });

    splitTimelineOpenGL->setSizes({ width() * 3 / 4, width() / 4 });

    TimelineGl::getInstance().set_to_part();

    for (ToolButton* item : toolControlSliderList) {
        ui->mainToolBar->addAction(item->action());
    }

    for (ToolButton* item : toolControlSliderList) {
        if (item->isSelect()) {
            PiYingGL::getInstance().setChTool(item->toolState());
            return;
        }
    }

    PiYingGL::getInstance().setChTool(CharacterToolState::None);
}

void PiYing::update_part_slider()
{
    if (PiYingGL::getInstance().editMode != EditMode::controlSlide) return;

    Part* part = TimelineGl::getInstance().get_current_part();
    if (part) {
        if (splitListOpenGL->widget(0) == voidListWidget) {
            splitListOpenGL->widget(0)->setParent(nullptr);
            splitListOpenGL->insertWidget(0, sliderWidget);
        }

        sliderWidget->delete_all_layout();
        sliderWidget->add_slider_by_part(part);
    }
    else if (splitListOpenGL->widget(0) != voidListWidget) {
        splitListOpenGL->widget(0)->setParent(nullptr);
        splitListOpenGL->insertWidget(0, voidListWidget);
    }

    splitListOpenGL->setSizes({ width() / 5, width() * 4 / 5 });
}

void PiYing::onModeChanged(int mode)
{
    ui->mainToolBar->clear();
    map_change_edit(mode)();
}