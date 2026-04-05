#pragma once

#include <qobject>

class ToolButton;
class QToolBar;

class ChSkelenToolbar final : public QObject
{
public:
    ChSkelenToolbar();

    void set_to_toolbar(QToolBar* toolbar);
    void remember_last_tool();
    void press_1();
    void press_3();

private:
    void click_select();

private:
    ToolButton* _select;
};
