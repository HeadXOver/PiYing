#pragma once

#include <QHBoxLayout>

class QLabel;
class QSlider;
class QPushButton;
class PiYingGL;
class CtrlSlideWidget;

class CtrlSlideLayout final : public QHBoxLayout
{
    Q_OBJECT
public:
    explicit CtrlSlideLayout(PiYingGL& gl, QString labelName, int min, int max, int defaultValue, int id);
    ~CtrlSlideLayout();

public:
    QLabel* label;
    QSlider* slider;
    QPushButton* rightButton;
    PiYingGL& piYingGL;
    int id_;
};
