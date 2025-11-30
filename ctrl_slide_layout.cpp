#include "ctrlSlideLayout.h"
#include "ctrlSlideWidget.h"
#include "slide_applier.h"

#include "piYingGL.h"

#include <qlabel>
#include <qslider>
#include <qpushbutton>
#include <qhboxlayout>
#include <qmessagebox>
#include <qmenu>

CtrlSlideLayout::CtrlSlideLayout(PiYingGL& gl, SlideApplier& slideApplier, QString labelName, int min, int max, int defaultValue, int id, QWidget* parent) :
    piYingGL(gl), 
    id_(id), QWidget(parent),
    slide_applier(slideApplier)
{
    layout = new QHBoxLayout(this);
    label = new QLabel(labelName, this);
    label->setStyleSheet("QLabel { background-color : white; color : black; }");
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(min, max);
    slider->setValue(defaultValue);
    rightButton = new QPushButton(this);
    QIcon icon(":/PiYing/setIcon.png");
    rightButton->setIcon(icon);
    rightButton->setIconSize(QSize(16, 16));
    rightButton->setFixedSize(QSize(16, 16));

    layout->addWidget(label);
    layout->addWidget(slider);
    layout->addWidget(rightButton);

    connect(slider, &QSlider::valueChanged, this, [this](int value)
        {
            piYingGL.controlSlide(slide_applier.get_traces(id_), value);
        }
    );

    setLayout(layout);
}

CtrlSlideLayout::~CtrlSlideLayout()
{
}
