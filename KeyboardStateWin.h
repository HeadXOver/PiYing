#pragma once

class KeyboardStateWin
{
public:
    
    static bool isKeyHeld(int qtKey);

    
    static bool isShiftHeld();
    static bool isCtrlHeld();
    static bool isAltHeld();
    static bool isMetaHeld();

private:
    static int qtKeyToVirtualKey(int qtKey);
};