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

    void set_toolState(CharacterToolState state);

    void update();

    QAction* action() const;
    CharacterToolState toolState() const;
    bool isSelect() const;

private:
    QAction* action_;
    std::unique_ptr<QIcon> selected_;
    std::unique_ptr<QIcon> unselected_;
    CharacterToolState toolState_;
    bool isSelect_ = false;
};

