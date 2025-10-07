#include "ctrlSlideLayout.h"

CtrlSlideLayout::CtrlSlideLayout(QWidget* parent, QString labelName, int min, int max, int defaultValue)
{
    label = new QLabel(labelName, parent);
    slider = new QSlider(Qt::Horizontal, parent);
    slider->setRange(min, max);
    slider->setValue(defaultValue);
    rightButton = new QPushButton(parent);
    rightButton->setIcon(QIcon(":/PiYing/setIcon.png"));

    addWidget(label);
    addWidget(slider);
    addWidget(rightButton);
}