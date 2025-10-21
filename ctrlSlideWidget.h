#pragma once

#include <QWidget>

class CtrlSlideLayout;
class QVBoxLayout;
class PiYingGL;

class CtrlSlideWidget : public QWidget
{
    Q_OBJECT

public:
    CtrlSlideWidget(PiYingGL& gl, const QString& name, QWidget* parent = nullptr);
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
    PiYingGL& piYingGL;
};