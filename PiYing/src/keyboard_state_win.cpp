#include "KeyboardStateWin.h"

#include <windows.h>

#include <Qt>

bool KeyboardStateWin::isKeyHeld(int qtKey)
{
    int vk = qtKeyToVirtualKey(qtKey);
    return vk >= 0 && (GetAsyncKeyState(vk) & 0x8000);
}

bool KeyboardStateWin::isShiftHeld()
{
    return GetAsyncKeyState(VK_SHIFT) & 0x8000;
}

bool KeyboardStateWin::isCtrlHeld()
{
    return GetAsyncKeyState(VK_CONTROL) & 0x8000;
}

bool KeyboardStateWin::isAltHeld()
{
    return GetAsyncKeyState(VK_MENU) & 0x8000;
}

bool KeyboardStateWin::isMetaHeld()
{
    return (GetAsyncKeyState(VK_LWIN) | GetAsyncKeyState(VK_RWIN)) & 0x8000;
}

int KeyboardStateWin::qtKeyToVirtualKey(int qtKey)
{

    switch (qtKey) {
    case Qt::Key_Shift:      return VK_SHIFT;
    case Qt::Key_Control:    return VK_CONTROL;
    case Qt::Key_Alt:        return VK_MENU;
    case Qt::Key_Meta:       return VK_LWIN;
    case Qt::Key_Space:      return VK_SPACE;
    case Qt::Key_Return:
    case Qt::Key_Enter:      return VK_RETURN;
    case Qt::Key_Escape:     return VK_ESCAPE;
    case Qt::Key_Tab:        return VK_TAB;
    case Qt::Key_Backspace:  return VK_BACK;
    case Qt::Key_Delete:     return VK_DELETE;
    case Qt::Key_Home:       return VK_HOME;
    case Qt::Key_End:        return VK_END;
    case Qt::Key_PageUp:     return VK_PRIOR;
    case Qt::Key_PageDown:   return VK_NEXT;
    case Qt::Key_Left:       return VK_LEFT;
    case Qt::Key_Right:      return VK_RIGHT;
    case Qt::Key_Up:         return VK_UP;
    case Qt::Key_Down:       return VK_DOWN;
    default:

        if (qtKey >= Qt::Key_A && qtKey <= Qt::Key_Z)
            return 'A' + (qtKey - Qt::Key_A);
        if (qtKey >= Qt::Key_0 && qtKey <= Qt::Key_9)
            return '0' + (qtKey - Qt::Key_0);
        return -1;
    }
}
