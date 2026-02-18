#pragma once

#include <qwidget>

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
    void add_slider_by_part(Part* part);

    QList<QString> get_slider_names();

private:
    void setSlider(size_t sliderIndex);
    void setName(size_t sliderIndex);

    QString get_unique_name(const QString& str, size_t sliderIndex);

private:
    QList<CtrlSlideLayout*> sliderList;

    QVBoxLayout* sliderLayout = nullptr;
};