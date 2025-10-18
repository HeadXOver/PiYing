#include "ctrlSlideLayout.h"
#include "ctrlSlideWidget.h"

#include "piYingGL.h"

#include <qlabel>
#include <qslider>
#include <qpushbutton>
#include <qmessagebox>
#include <qmenu>

CtrlSlideLayout::CtrlSlideLayout(PiYingGL& gl, QString labelName, int min, int max, int defaultValue, int id) :piYingGL(gl), id_(id)
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

    connect(slider, &QSlider::valueChanged, this, [this](int value) { piYingGL.controlSlide(id_, value); });
}

CtrlSlideLayout::~CtrlSlideLayout()
{
    delete label;
    delete slider;
    delete rightButton;
}
