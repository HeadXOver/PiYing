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

    void addSlider(QString name = "Slider");
    void delete_all_layout();
    void add_slider_by_part(std::shared_ptr<Part> part);

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
};