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
    explicit CtrlSlideLayout(QWidget* parent, QString labelName, int min, int max, int defaultValue);

public:
    QLabel* label;
    QSlider* slider;
    QPushButton* rightButton;
};
