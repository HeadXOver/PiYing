#pragma once

#include <QHBoxLayout>

class QLabel;
class QSlider;
class QPushButton;

class CtrlSlideLayout : public QHBoxLayout
{
    Q_OBJECT
public:
    explicit CtrlSlideLayout(QString labelName, int min, int max, int defaultValue);
    ~CtrlSlideLayout();

public:
    QLabel* label;
    QSlider* slider;
    QPushButton* rightButton;
};
