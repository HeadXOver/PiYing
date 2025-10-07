#pragma once

#include <QWidget>

#include "ctrlSlideLayout.h"

class CtrlSlideWidget : public QWidget
{
    Q_OBJECT

public:
    CtrlSlideWidget(QWidget* parent = nullptr);

    void addSlider(CtrlSlideLayout* slider, QString name = "Slider");

private:
    QList<CtrlSlideLayout*> sliderList;
    QVBoxLayout* sliderLayout = nullptr;
    int sliderCount = 0;
};