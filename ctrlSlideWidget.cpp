#include "ctrlSlideWidget.h"

#include <QPushbutton>
#include <QVBoxLayout>
#include <QString>

CtrlSlideWidget::CtrlSlideWidget(QWidget* parent)
{
    sliderLayout = new QVBoxLayout();

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

    sliderLayout->addWidget(headButton);
    sliderLayout->addStretch();

    setLayout(sliderLayout);
}

void CtrlSlideWidget::addSlider(CtrlSlideLayout* slider, QString name)
{
    sliderList.append(new CtrlSlideLayout(this, name, 0, 100, 50));

    sliderLayout->insertLayout(sliderCount, sliderList[sliderCount++]);
}
