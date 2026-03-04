#pragma once

#include <qicon>

class QAction;
class QWidget;
class QString;

enum class CharacterToolState;

class ToolButton {
public:
    ToolButton(const QString& selectedFileName, const QString& unselectedFileName, const QString& actionName, CharacterToolState state);
    ~ToolButton();

    void select();
    void unSelect();

    void set_selected(const QIcon& icon);
    void set_unselected(const QIcon& icon);

    void set_selected(const QString& iconFile);
    void set_unselected(const QString& iconFile);

    void set_toolState(CharacterToolState state) noexcept;

    void update();

    QAction* action() const noexcept { return action_; }
    CharacterToolState toolState() const noexcept { return toolState_; }
    bool isSelect() const noexcept { return isSelect_; }

private:
    QAction* action_;
    QIcon selected_;
    QIcon unselected_;
    CharacterToolState toolState_;
    bool isSelect_ = false;
};

