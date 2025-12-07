#include "PiYing.h"
#include "PiYingGL.h"
#include "tool_button.h"

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
            toolButton->set_selected(QIcon(":/PiYing/rect_triangle_select_S.png"));
            toolButton->set_unselected(QIcon(":/PiYing/rect_triangle_select.png"));
            toolButton->set_toolState(CharacterToolState::LibreSelectTriangle);
        }break;
        case CharacterToolState::LibreSelectTriangle: {
            toolButton->set_selected(QIcon(":/PiYing/libre_triangle_select_S.png"));
            toolButton->set_unselected(QIcon(":/PiYing/libre_triangle_select.png"));
            toolButton->set_toolState(CharacterToolState::RectSelectTriangle);
            toolButton->unSelect();
            piYingGL->setChTool(CharacterToolState::None);
            return;
        }break;
        default: return;
        }
    }
    else {
        for (ToolButton* item : toolChTexList) item->unSelect();
        for (ToolButton* item : toolControlSliderList) item->unSelect();
        for (ToolButton* item : toolChSkelenList) item->unSelect();
    }

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