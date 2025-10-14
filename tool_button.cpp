#include "tool_button.h"

#include <qwidget>
#include <qaction>

ToolButton::ToolButton(QString selectedFileName, QString unselectedFileName, QString actionName, CharacterTextureToolState state, QWidget* parent) 
{
    selected_ = QIcon(selectedFileName);
    unselected_ = QIcon(unselectedFileName);
    action_ = new QAction(unselected_, actionName, parent);
    toolState_ = state;
}

void ToolButton::select() 
{
    action_->setIcon(selected_);
    isSelect_ = true;
}

void ToolButton::unSelect()
{
    action_->setIcon(unselected_);
    isSelect_ = false;
}

void ToolButton::set_selected(QIcon icon)
{
    selected_ = icon;
}

void ToolButton::set_unselected(QIcon icon)
{
    unselected_ = icon;
}

void ToolButton::set_toolState(CharacterTextureToolState state)
{
    toolState_ = state;
}

QAction* ToolButton::action() const
{
    return action_;
}

CharacterTextureToolState ToolButton::toolState() const
{
    return toolState_;
}

bool ToolButton::isSelect() const
{
    return isSelect_;
}
