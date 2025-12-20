#pragma once

#include <qwidget>
#include <memory>

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
    explicit CtrlSlideLayout(QString labelName, unsigned int id, int defaultValue, QWidget* parent);
    ~CtrlSlideLayout();

public:
    QHBoxLayout* layout;
    QLabel* label;
    QSlider* slider;
    QPushButton* rightButton;

    int id_;
};
;