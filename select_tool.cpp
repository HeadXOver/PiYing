#include "PiYing.h"
#include "PiYingGL.h"
#include "global_objects.h"
#include "tool_button.h"
#include "ui_PiYing.h"

void PiYing::select_tool_texture(ToolButton* toolButton)
{
    if (toolButton->isSelect()) {
        switch (toolButton->toolState()) {
        case CharacterToolState::RectSelectVert: {
            toolButton->set_selected(QIcon(":/PiYing/selectLibreChVert_S.png"));
            toolButton->set_unselected(QIcon(":/PiYing/selectLibreChVert.png"));
            toolButton->set_toolState(CharacterToolState::LibreSelectVert);
        }break;
        case CharacterToolState::LibreSelectVert: {
            toolButton->set_selected(QIcon(":/PiYing/selectRectChVert_S.png"));
            toolButton->set_unselected(QIcon(":/PiYing/selectRectChVert.png"));
            toolButton->set_toolState(CharacterToolState::RectSelectVert);
            toolButton->unSelect();
            piYingGL->setChTool(CharacterToolState::None);
            return;
        }break;
        case CharacterToolState::RectSelectTriangle: {
            toolButton->set_selected(QIcon(":/PiYing/libre_triangle_select_S.png"));
            toolButton->set_unselected(QIcon(":/PiYing/libre_triangle_select.png"));
            toolButton->set_toolState(CharacterToolState::LibreSelectTriangle);
        }break;
        case CharacterToolState::LibreSelectTriangle: {
            toolButton->set_selected(QIcon(":/PiYing/rect_triangle_select_S.png"));
            toolButton->set_unselected(QIcon(":/PiYing/rect_triangle_select.png"));
            toolButton->set_toolState(CharacterToolState::RectSelectTriangle);
            toolButton->unSelect();
            piYingGL->setChTool(CharacterToolState::None);
            return;
        }break;
        default: return;
        }
    }
    else all_button_unselect();

    toolButton->select();
    piYingGL->setChTool(toolButton->toolState());
}

void PiYing::select_tool_skelen(ToolButton* toolButton)
{
    if (toolButton->isSelect()) return; 

    for (ToolButton* item : toolChSkelenList) item->unSelect();

    toolButton->select();
    piYingGL->setChTool(toolButton->toolState());
}

void PiYing::select_tool_control_slider(ToolButton* toolButton)
{
    if (toolButton->isSelect()) {
        toolButton->unSelect();
        piYingGL->setChTool(CharacterToolState::None);
        return;
    }

    for (ToolButton* item : toolControlSliderList) item->unSelect();

    toolButton->select();
    piYingGL->setChTool(toolButton->toolState());
}

void PiYing::adapt_select_tool_button(CharacterToolState state)
{
    switch (state) {
    case CharacterToolState::RectSelectVert: {
        _select_button->set_selected(QIcon(":/PiYing/selectRectChVert_S.png"));
        _select_button->set_unselected(QIcon(":/PiYing/selectRectChVert.png"));
        _select_button->set_toolState(CharacterToolState::RectSelectVert);
        _select_button->update();
    }break;
    case CharacterToolState::LibreSelectVert: {
        _select_button->set_selected(QIcon(":/PiYing/selectLibreChVert_S.png"));
        _select_button->set_unselected(QIcon(":/PiYing/selectLibreChVert.png"));
        _select_button->set_toolState(CharacterToolState::LibreSelectVert);
        _select_button->update();
    }break;
    case CharacterToolState::RectSelectTriangle: {
        _select_button->set_selected(QIcon(":/PiYing/rect_triangle_select_S.png"));
        _select_button->set_unselected(QIcon(":/PiYing/rect_triangle_select.png"));
        _select_button->set_toolState(CharacterToolState::RectSelectTriangle);
        _select_button->update();
    }break;
    case CharacterToolState::LibreSelectTriangle: {
        _select_button->set_selected(QIcon(":/PiYing/libre_triangle_select_S.png"));
        _select_button->set_unselected(QIcon(":/PiYing/libre_triangle_select.png"));
        _select_button->set_toolState(CharacterToolState::LibreSelectTriangle);
        _select_button->update();
    }break;
    }
}

void PiYing::all_button_unselect()
{
    for (ToolButton* item : toolChTexList) item->unSelect();
    for (ToolButton* item : toolControlSliderList) item->unSelect();
    for (ToolButton* item : toolChSkelenList) item->unSelect();
}
