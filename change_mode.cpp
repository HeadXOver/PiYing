#include "PiYing.h"
#include "PiYingGL.h"
#include "global_objects.h"
#include "tool_button.h"
#include "ctrlSlideWidget.h"
#include "time_line_gl.h"
#include "enum_edit_mode.h"

#include "ui_PiYing.h"

#include <qsplitter>
#include <qlistwidget>

namespace {

    constexpr void(*change_edit_mode[])(PiYing* piYing) = {
        [](PiYing* piYing) { piYing->change_edit_mode_overview(); },
        [](PiYing* piYing) { piYing->change_edit_mode_background(); },
        [](PiYing* piYing) { piYing->change_edit_mode_character_texture(); },
        [](PiYing* piYing) { piYing->change_edit_mode_character_skeleton(); },
        [](PiYing* piYing) { piYing->change_edit_mode_character_constrol_slider(); }
    };

    using change_edit_handler = void(*)(PiYing*);
    constexpr change_edit_handler map_change_edit(int state) {
        return change_edit_mode[state];
    }

}

void PiYing::change_edit_mode_overview()
{
    splitListOpenGL->widget(0)->setParent(nullptr);
    splitListOpenGL->insertWidget(0, voidListWidget);

    splitTimelineOpenGL->setSizes({ width() * 5 / 6, width() / 6 });

    timeLineGL->set_to_timeline();

    piYingGL->setChTool(CharacterToolState::None);
    piYingGL->setEditMode(EditMode::OverView);
}

void PiYing::change_edit_mode_background()
{
    splitListOpenGL->widget(0)->setParent(nullptr);
    splitListOpenGL->insertWidget(0, bgImageList);

    splitTimelineOpenGL->setSizes({ width() * 5 / 6, width() / 6 });

    timeLineGL->set_to_timeline();

    piYingGL->setChTool(CharacterToolState::None);
    piYingGL->setEditMode(EditMode::BackGround);
}

void PiYing::change_edit_mode_character_texture()
{
    splitListOpenGL->widget(0)->setParent(nullptr);
    splitListOpenGL->insertWidget(0, chImageList);

    splitTimelineOpenGL->setSizes({ width() * 3 / 4, width() / 4 });

    timeLineGL->set_to_part();

    piYingGL->setEditMode(EditMode::characterTexture);

    for (ToolButton* item : toolChTexList) {
        ui->mainToolBar->addAction(item->action());
    }

    for (ToolButton* item : toolChTexList) {
        if (item->isSelect()) {
            piYingGL->setChTool(item->toolState());
            return;
        }
    }

    if(!piYingGL->have_ch_tool()) piYingGL->setChTool(CharacterToolState::None);
}

void PiYing::change_edit_mode_character_skeleton()
{
    splitListOpenGL->widget(0)->setParent(nullptr);
    splitListOpenGL->insertWidget(0, chImageList);

    splitTimelineOpenGL->setSizes({ width() * 3 / 4, width() / 4 });

    timeLineGL->set_to_part();

    piYingGL->setEditMode(EditMode::characterSkeleton);

    for (ToolButton* item : toolChSkelenList) {
        ui->mainToolBar->addAction(item->action());
    }

    for (ToolButton* item : toolChSkelenList) {
        if (item->isSelect()) {
            piYingGL->setChTool(item->toolState());
            return;
        }
    }

    if (!piYingGL->have_ch_tool()) piYingGL->setChTool(CharacterToolState::None);
}

void PiYing::change_edit_mode_character_constrol_slider()
{
    splitListOpenGL->widget(0)->setParent(nullptr);
    if(getCurrentChRow() >= 0)
        splitListOpenGL->insertWidget(0, sliderWidget[getCurrentChRow()]);
    else
        splitListOpenGL->insertWidget(0, voidListWidget);
    piYingGL->setEditMode(EditMode::controlSlide);
    splitListOpenGL->setSizes({ width() / 5, width() * 4 / 5 });

    splitTimelineOpenGL->setSizes({ width() * 3 / 4, width() / 4 });

    timeLineGL->set_to_part();

    for (ToolButton* item : toolControlSliderList) {
        ui->mainToolBar->addAction(item->action());
    }

    for (ToolButton* item : toolControlSliderList) {
        if (item->isSelect()) {
            piYingGL->setChTool(item->toolState());
            return;
        }
    }

    if (!piYingGL->have_ch_tool()) piYingGL->setChTool(CharacterToolState::None);
}

void PiYing::update_timeline()
{
    timeLineGL->update();
}

void PiYing::onModeChanged(int mode)
{
    ui->mainToolBar->clear();
    map_change_edit(mode)(this);
}