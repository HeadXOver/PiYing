#include "ctrlSlideLayout.h"

CtrlSlideLayout::CtrlSlideLayout(QString labelName, int min, int max, int defaultValue)
{
    label = new QLabel(labelName);
    slider = new QSlider(Qt::Horizontal);
    slider->setRange(min, max);
    slider->setValue(defaultValue);
    rightButton = new QPushButton();
    QIcon icon(":/PiYing/setIcon.png");
    rightButton->setIcon(icon);
    rightButton->setIconSize(QSize(16, 16));
    rightButton->setFixedSize(QSize(16, 16));

    addWidget(label);
    addWidget(slider);
    addWidget(rightButton);
}

CtrlSlideLayout::~CtrlSlideLayout()
{
    delete label;
    delete slider;
    delete rightButton;
}
