#include "ctrlSlideLayout.h"
#include "ctrlSlideWidget.h"
#include "slide_applier.h"

#include "time_line_gl.h"
#include "part.h"
#include "global_objects.h"

#include <qlabel>
#include <qslider>
#include <qpushbutton>
#include <qhboxlayout>
#include <qmessagebox>
#include <qmenu>

CtrlSlideLayout::CtrlSlideLayout(QString labelName, unsigned int id, int defaultValue, QWidget* parent) :
    id_(id), QWidget(parent)
{
    layout = new QHBoxLayout(this);
    label = new QLabel(labelName, this);
    label->setStyleSheet("QLabel { background-color : white; color : black; }");
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 1000);
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
            std::shared_ptr<Part> part = timelineGl->get_current_part();
            if(part) part->change_slider_value(id_, value);
        }
    );

    setLayout(layout);
}

CtrlSlideLayout::~CtrlSlideLayout()
{
}
