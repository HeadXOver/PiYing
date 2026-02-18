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

void ToolButton::set_toolState(CharacterToolState state) noexcept
{
    toolState_ = state;
}

void ToolButton::update()
{
    action_->setIcon(isSelect_ ? *selected_ : *unselected_);
}
