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
    explicit CtrlSlideLayout(PiYingGL& gl, SlideApplier& slideApplier, QString labelName, int min, int max, int defaultValue, int id, QWidget* parent);
    ~CtrlSlideLayout();

public:
    QHBoxLayout* layout;
    QLabel* label;
    QSlider* slider;
    QPushButton* rightButton;
    PiYingGL& piYingGL;
    SlideApplier& slide_applier;
    int id_;
};
;