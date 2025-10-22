#include "tool_button.h"

#include <qwidget>
#include <qaction>
#include <qicon>

ToolButton::ToolButton(const QString& selectedFileName, const QString& unselectedFileName, const QString& actionName, CharacterToolState state, QWidget* parent)
{
    selected_ = std::make_unique<QIcon>(selectedFileName);
    unselected_ = std::make_unique<QIcon>(unselectedFileName);
    action_ = new QAction(*unselected_, actionName, parent);
    toolState_ = state;
}

ToolButton::~ToolButton()
{
}

void ToolButton::select() 
{
    action_->setIcon(*selected_);
    isSelect_ = true;
}

void ToolButton::unSelect()
{
    action_->setIcon(*unselected_);
    isSelect_ = false;
}

void ToolButton::set_selected(const QIcon& icon)
{
    *selected_ = icon;
}

void ToolButton::set_unselected(const QIcon& icon)
{
    *unselected_ = icon;
}

void ToolButton::set_toolState(CharacterToolState state)
{
    toolState_ = state;
}

QAction* ToolButton::action() const
{
    return action_;
}

CharacterToolState ToolButton::toolState() const
{
    return toolState_;
}

bool ToolButton::isSelect() const
{
    return isSelect_;
}
