#pragma once

#include <memory>

class QAction;
class QWidget;
class QIcon;
class QString;

enum class CharacterToolState;

class ToolButton {
public:
    ToolButton(const QString& selectedFileName, const QString& unselectedFileName, const QString& actionName, CharacterToolState state, QWidget* parent);
    ~ToolButton();

    void select();
    void unSelect();

    void set_selected(const QIcon& icon);
    void set_unselected(const QIcon& icon);

    void set_toolState(CharacterToolState state) noexcept;

    void update();

    QAction* action() const noexcept { return action_; }
    CharacterToolState toolState() const noexcept { return toolState_; }
    bool isSelect() const noexcept { return isSelect_; }

private:
    QAction* action_;
    std::unique_ptr<QIcon> selected_;
    std::unique_ptr<QIcon> unselected_;
    CharacterToolState toolState_;
    bool isSelect_ = false;
};

