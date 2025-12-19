#include "ctrlSlideWidget.h"
#include "ctrlSlideLayout.h"

#include "cus_func_get_unique_id.h"

#include "piYingGL.h"
#include "part.h"
#include "time_line_gl.h"
#include "slide_applier.h"
#include "global_objects.h"

#include <QPushbutton>
#include <QBoxLayout>
#include <QMenu>
#include <QInputDialog>
#include <qlabel>
#include <qslider>

CtrlSlideWidget::CtrlSlideWidget() : QWidget(piYingGL)
{
    sliderLayout = new QVBoxLayout(this);

    QPushButton* addButton = new QPushButton(this);
    addButton->setText("添加");

    addButton->setStyleSheet(
        "QPushButton {"
        "   background-color: white;"
        "   color: black;"
        "   border: 1px solid gray;"
        "   padding: 5px;"
        "}"
    );

    QHBoxLayout* headLayout = new QHBoxLayout(this);

    headLayout->addWidget(addButton);
    headLayout->addStretch();

    sliderLayout->addLayout(headLayout);
    sliderLayout->addStretch();

    setLayout(sliderLayout);

    connect(addButton, &QPushButton::pressed, this, [this] { addSlider(); });
}

CtrlSlideWidget::~CtrlSlideWidget()
{
    for (int i = 0; i < sliderList.size(); i++) {
        delete sliderList[i];
    }
}

void CtrlSlideWidget::addSlider(QString name)
{
    CtrlSlideLayout* ctrlSlideLayout = new CtrlSlideLayout(timelineGl->get_current_part()->get_slide_applier(), get_unique_name(name), 0, 100, 0, get_unique_id(sliderList), this);
    sliderList.append(ctrlSlideLayout);

    sliderLayout->insertWidget(sliderCount, sliderList[sliderCount++]);
    connect(ctrlSlideLayout->rightButton,   &QPushButton::pressed, this, [ctrlSlideLayout, this] { setSlider(ctrlSlideLayout); });
}

bool CtrlSlideWidget::add_trace(int id, int index, const QPolygonF& trace) 
{
    return timelineGl->get_current_part()->get_slide_applier().add_trace2(id, index, trace);
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
        if (sliderList[i] != slider) continue;

        sliderList.removeAt(i);
        sliderLayout->removeWidget(slider);
        timelineGl->get_current_part()->get_slide_applier().remove_slider_by_id(slider->id_);
        delete slider;
        sliderCount--;
        return;
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

    if (ok) slider->label->setText(get_unique_name(text));
}

QString CtrlSlideWidget::get_unique_name(const QString& str)
{
    QString name;
    for (int i = 0;; i++) {
        if(i) name = str + QString::number(i);
        else name = str;
        bool isExist = false;
        for (int j = 0; j < sliderList.size(); j++) {
            if (sliderList[j]->label->text() == name) {
                isExist = true;
                break;
            }
        }
        if (!isExist) break;
    }
    return name;
}

QList<QString> CtrlSlideWidget::get_slider_names()
{
    QList<QString> names;
    for (int i = 0; i < sliderList.size(); i++) {
        names.append(sliderList[i]->label->text());
    }
    return names;
}

int CtrlSlideWidget::get_id(int index) const 
{
    return sliderList[index]->id_;
}
