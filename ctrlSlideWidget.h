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

    QList<QString> get_slider_names();
    
    int get_id(int index) const;

private:
    void setSlider(CtrlSlideLayout* slider);
    void removeSlider(CtrlSlideLayout* slider);
    void setName(CtrlSlideLayout* slider);

    QString get_unique_name(const QString& str);

private:
    QList<CtrlSlideLayout*> sliderList;

    QVBoxLayout* sliderLayout = nullptr;
    int sliderCount = 0;
    PiYingGL& piYingGL;
};