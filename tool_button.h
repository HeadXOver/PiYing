#pragma once

#include <qstring>
#include <qicon>

class QAction;
class QWidget;

enum class ChTexToolState {
    None,
    AddTriangle,
    MoveVert,
    RectSelectVert,
    LibreSelectVert,
    AddPoly,
};

class ToolButton {
public:
    ToolButton(QString selectedFileName, QString unselectedFileName, QString actionName, ChTexToolState state, QWidget* parent);

    void select();
    void unSelect();

    void set_selected(QIcon icon);
    void set_unselected(QIcon icon);

    void set_toolState(ChTexToolState state);

    QAction* action() const;
    ChTexToolState toolState() const;
    bool isSelect() const;

private:
    QAction* action_;
    QIcon selected_;
    QIcon unselected_;
    ChTexToolState toolState_;
    bool isSelect_ = false;
};

