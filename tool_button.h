#pragma once

#include "enum_character_texture_tool_state.h"

class QAction;
class QWidget;
class QIcon;
class QString;

class ToolButton {
public:
    ToolButton(const QString& selectedFileName, const QString& unselectedFileName, const QString& actionName, CharacterToolState state, QWidget* parent);
    ~ToolButton();

    void select();
    void unSelect();

    void set_selected(const QIcon& icon);
    void set_unselected(const QIcon& icon);

    void set_toolState(CharacterToolState state);

    QAction* action() const;
    CharacterToolState toolState() const;
    bool isSelect() const;

private:
    QAction* action_;
    QIcon* selected_;
    QIcon* unselected_;
    CharacterToolState toolState_;
    bool isSelect_ = false;
};

