#include "ctrlSlideWidget.h"
#include "ctrlSlideLayout.h"

#include "cus_func_get_unique_id.h"

#include "piYingGL.h"

#include <QPushbutton>
#include <QBoxLayout>
#include <QMenu>
#include <QInputDialog>
#include <qlabel>
#include <qslider>

CtrlSlideWidget::CtrlSlideWidget(PiYingGL& gl, QWidget* parent) : QWidget(parent), piYingGL(gl)
{
    sliderLayout = new QVBoxLayout(this);

    QPushButton* headButton = new QPushButton(this);
    headButton->setText("添加");

    headButton->setStyleSheet(
        "QPushButton {"
        "   background-color: white;"
        "   color: black;"
        "   border: 1px solid gray;"
        "   padding: 5px;"
        "}"
    );

    QHBoxLayout* headLayout = new QHBoxLayout(this);

    headLayout->addWidget(headButton);
    headLayout->addStretch();

    sliderLayout->addLayout(headLayout);
    sliderLayout->addStretch();

    setLayout(sliderLayout);

    connect(headButton, &QPushButton::pressed, this, [this] { addSlider(); });
}

CtrlSlideWidget::~CtrlSlideWidget()
{
    for (int i = 0; i < sliderList.size(); i++) {
        delete sliderList[i];
    }
}

void CtrlSlideWidget::addSlider(QString name)
{
    CtrlSlideLayout* ctrlSlideLayout = new CtrlSlideLayout(piYingGL, name, 0, 100, 50, get_unique_id(sliderList));
    sliderList.append(ctrlSlideLayout);

    sliderLayout->insertLayout(sliderCount, sliderList[sliderCount++]);
    connect(ctrlSlideLayout->rightButton,   &QPushButton::pressed, this, [ctrlSlideLayout, this] { setSlider(ctrlSlideLayout); });
}

void CtrlSlideWidget::setSlider(CtrlSlideLayout* slider)
{
    QMenu* menu = new QMenu(this);
    menu->addAction("删除", this, [slider, this] { removeSlider(slider); });
    menu->addAction("重命名", this, [slider, this] { setName(slider); });
    menu->exec(QCursor::pos());
}

void CtrlSlideWidget::removeSlider(CtrlSlideLayout* slider)
{
    for (int i = 0; i < sliderList.size(); i++) {
        if (sliderList[i] == slider) {
            sliderList.removeAt(i);
            sliderLayout->removeItem(slider);
            delete slider;
            sliderCount--;
            break;
        }
    }
}

void CtrlSlideWidget::setName(CtrlSlideLayout* slider)
{
    bool ok;
    QString text = QInputDialog::getText(
        this,
        " 输入名称",
        "请输入名称：",
        QLineEdit::Normal,
        "slider",
        &ok
    );

    if (ok) slider->label->setText(text);
}

