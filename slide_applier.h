#pragma once

#include <map>
#include <memory>
#include <vector>
#include <qstring>

class CharacterTrace;
class QPolygonF;

class SlideApplier final
{
public:
    SlideApplier();
    SlideApplier(const SlideApplier& applier1, const SlideApplier& applier2, unsigned int skew);
    ~SlideApplier();

    void remove_slider_by_id(int id);

    bool add_trace_on_exist_slider(int sliderId, int index, const QPolygonF& polygon);
    void add_new_slider(int index, const QPolygonF& polygon);
    const std::map<unsigned int, QPolygonF>& get_trace_map(int slide) const;

    unsigned int n_sliders() const;

    const QString& get_slider_name(int id) const;

    QString get_unique_name(const QString& str) const;
    QString get_unique_name_except(const QString& str, int exceptId) const;

    void change_current_value(int sliderIndex, int value);
    void remove_slider(int sliderIndex);
    void eat_other_sliders(SlideApplier* other, unsigned int skew);

    int get_slider_current_value(int sliderIndex) const;

private:
    std::vector<std::unique_ptr<CharacterTrace>> sliders;
};

