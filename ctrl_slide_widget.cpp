#include "ctrlSlideWidget.h"
#include "ctrlSlideLayout.h"

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
}

CtrlSlideWidget::~CtrlSlideWidget()
{
    for (int i = 0; i < sliderList.size(); i++) {
        delete sliderList[i];
    }
}

void CtrlSlideWidget::setSlider(unsigned int sliderIndex)
{
    QMenu* menu = new QMenu(this);
    menu->addAction("删除", this, [sliderIndex, this] { timelineGl->get_current_part()->remove_slider(sliderIndex); });
    menu->addAction("重命名", this, [sliderIndex, this] { setName(sliderIndex); });
    menu->exec(QCursor::pos());
}

void CtrlSlideWidget::setName(unsigned int sliderIndex)
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

    if (ok) sliderList[sliderIndex]->label->setText(get_unique_name(text));
}

void CtrlSlideWidget::delete_all_layout()
{
    for (QWidget* slider : sliderList) {
        sliderLayout->removeWidget(slider);  // 从布局中移除
        delete slider;                       // 删除控件
    }
    sliderList.clear();
}

void CtrlSlideWidget::add_slider_by_part(std::shared_ptr<Part> part)
{
    SlideApplier& applier = part->get_slide_applier();

    const unsigned int nApplier = applier.n_sliders();

    for (unsigned int i = 0; i < nApplier; i++) {
        CtrlSlideLayout* ctrlSlideLayout = new CtrlSlideLayout(applier.get_slider_name(i), i, applier.get_slider_current_value(i), this);
        sliderList.push_back(ctrlSlideLayout);

        sliderLayout->insertWidget(i + 1, ctrlSlideLayout);
        connect(ctrlSlideLayout->rightButton, &QPushButton::pressed, this, [i, this] { setSlider(i); });
    }
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
