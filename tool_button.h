#pragma once

#include <qicon>

#include "enum_character_texture_tool_state.h"

class QAction;
class QWidget;

class ToolButton {
public:
    ToolButton(QString selectedFileName, QString unselectedFileName, QString actionName, CharacterTextureToolState state, QWidget* parent);

    void select();
    void unSelect();

    void set_selected(QIcon icon);
    void set_unselected(QIcon icon);

    void set_toolState(CharacterTextureToolState state);

    QAction* action() const;
    CharacterTextureToolState toolState() const;
    bool isSelect() const;

private:
    QAction* action_;
    QIcon selected_;
    QIcon unselected_;
    CharacterTextureToolState toolState_;
    bool isSelect_ = false;
};

