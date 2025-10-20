#include "PiYing.h"
#include "PiYingGL.h"
#include "tool_button.h"
#include "ctrlSlideWidget.h"

#include "ui_PiYing.h"

#include <qsplitter>
#include <qlistwidget>

void PiYing::change_edit_mode_overview()
{
}

void PiYing::change_edit_mode_character_texture()
{
}

void PiYing::change_edit_mode_character_skeleton()
{
}

void PiYing::change_edit_mode_background()
{
}

void PiYing::change_edit_mode_character_constrol_slider()
{
}

void PiYing::onModeChanged(int mode)
{
    ui->mainToolBar->clear();

    if (mode == 0) {
        splitListOpenGL->widget(0)->setParent(nullptr);
        splitListOpenGL->insertWidget(0, voidListWidget);
        piYingGL->setChTool(CharacterToolState::None);
        piYingGL->setEditMode(EditMode::Default);
    }
    else if (mode == 1) {
        splitListOpenGL->widget(0)->setParent(nullptr);
        splitListOpenGL->insertWidget(0, bgImageList);
        piYingGL->setChTool(CharacterToolState::None);
        piYingGL->setEditMode(EditMode::BackGround);
    }
    else if (mode == 2) {
        splitListOpenGL->widget(0)->setParent(nullptr);
        splitListOpenGL->insertWidget(0, chImageList);
        piYingGL->setEditMode(EditMode::characterTexture);

        for (ToolButton* item : toolChTexList) {
            ui->mainToolBar->addAction(item->action());
        }

        for (ToolButton* item : toolChTexList) {
            if (item->isSelect()) {
                piYingGL->setChTool(item->toolState());
                break;
            }
        }
    }
    else if (mode == 3) {
        splitListOpenGL->widget(0)->setParent(nullptr);
        splitListOpenGL->insertWidget(0, chImageList);
        piYingGL->setEditMode(EditMode::characterSkeleton);

        for (ToolButton* item : toolChSkelenList) {
            ui->mainToolBar->addAction(item->action());
        }

        for (ToolButton* item : toolChSkelenList) {
            if (item->isSelect()) {
                piYingGL->setChTool(item->toolState());
                break;
            }
        }
    }
    else if (mode == 4) {
        splitListOpenGL->widget(0)->setParent(nullptr);
        splitListOpenGL->insertWidget(0, sliderWidget);
        piYingGL->setEditMode(EditMode::controlSlide);
        splitListOpenGL->setSizes({ width() / 5, width() * 4 / 5 });

        for (ToolButton* item : toolControlSliderList) {
            ui->mainToolBar->addAction(item->action());
        }

        for (ToolButton* item : toolControlSliderList) {
            if (item->isSelect()) {
                piYingGL->setChTool(item->toolState());
                break;
            }
        }
    }
}