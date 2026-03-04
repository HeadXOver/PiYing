#include "PiYing.h"
#include "PiYingGL.h"
#include "tool_button.h"

#include "enum_character_texture_tool_state.h"

void PiYing::select_tool_control_slider(ToolButton* toolButton)
{
    if (toolButton->isSelect()) {
        toolButton->unSelect();
        PiYingGL::getInstance().setChTool(CharacterToolState::None);
        return;
    }

    //for (ToolButton* item : toolControlSliderList) item->unSelect();

    toolButton->select();
    PiYingGL::getInstance().setChTool(toolButton->toolState());
}