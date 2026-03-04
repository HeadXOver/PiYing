#pragma once

#include <qobject>

class ToolButton;
class QToolBar;

class ControlSliderToolbar final : public QObject
{
public:
    ControlSliderToolbar();

    void set_to_toolbar(QToolBar* toolbar);
    void remember_last_tool();
    void press_1();
    void press_3();

private:
    void click_select();
    void click_add_vert_trace();

private:
    ToolButton* _select;
    ToolButton* _add_vert_trace;
};