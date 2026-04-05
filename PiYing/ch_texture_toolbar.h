#pragma once

#include <qobject>

class ToolButton;
class QToolBar;

class ChTextureToolbar final : public QObject
{
public:
    ChTextureToolbar();

    void set_to_toolbar(QToolBar* toolbar);
    void remember_last_tool();
    void press_1();
    void press_3();

private:
    void click_select();
    void click_add_triangle();
    void click_add_poly();
    void click_add_round();

private:
    ToolButton* _select;
    ToolButton* _add_triangle;
    ToolButton* _add_poly;
    ToolButton* _add_round;
};
