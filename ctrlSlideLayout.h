#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QIcon>

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
