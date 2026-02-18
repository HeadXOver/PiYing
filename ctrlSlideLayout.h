#pragma once

#include <qwidget>

class QLabel;
class QSlider;
class QPushButton;
class PiYingGL;
class CtrlSlideWidget;
class QHBoxLayout;
class SlideApplier;

class CtrlSlideLayout final : public QWidget
{
    Q_OBJECT
public:
    explicit CtrlSlideLayout(QString labelName, size_t id, int defaultValue, QWidget* parent);
    ~CtrlSlideLayout();

public:
    QHBoxLayout* layout;
    QLabel* label;
    QSlider* slider;
    QPushButton* rightButton;
};