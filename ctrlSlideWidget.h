#pragma once

#include <QWidget>

class CtrlSlideLayout;
class QVBoxLayout;
class PiYingGL;
class SlideApplier;
class QPolygonF;
class QLabel;
class Part;

class CtrlSlideWidget : public QWidget
{
    Q_OBJECT

public:
    CtrlSlideWidget();
    ~CtrlSlideWidget();

    void delete_all_layout();
    void add_slider_by_part(std::shared_ptr<Part> part);

    QList<QString> get_slider_names();

private:
    void setSlider(unsigned int sliderIndex);
    void setName(unsigned int sliderIndex);

    QString get_unique_name(const QString& str);

private:
    QList<CtrlSlideLayout*> sliderList;

    QVBoxLayout* sliderLayout = nullptr;
};