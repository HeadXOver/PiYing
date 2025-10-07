#pragma once

#include <QWidget>

#include "ctrlSlideLayout.h"

class CtrlSlideWidget : public QWidget
{
    Q_OBJECT

public:
    CtrlSlideWidget(QWidget* parent = nullptr);
    ~CtrlSlideWidget();

    void addSlider(QString name = "Slider");

private:
    void setSlider(CtrlSlideLayout* slider);
    void removeSlider(CtrlSlideLayout* slider);
    void setName(CtrlSlideLayout* slider);

private:
    QList<CtrlSlideLayout*> sliderList;
    QVBoxLayout* sliderLayout = nullptr;
    int sliderCount = 0;
};