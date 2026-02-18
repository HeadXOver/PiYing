#pragma once

#include <unordered_map>
#include <memory>
#include <vector>

class CharacterTrace;
class QPolygonF;
class QString;

class SlideApplier final
{
public:
    SlideApplier();
    SlideApplier(const SlideApplier& applier1, const SlideApplier& applier2, unsigned int skew);
    SlideApplier(const SlideApplier& other);
    ~SlideApplier();

    void remove_slider_by_id(int id) noexcept;

    bool add_trace_on_exist_slider(int sliderId, int index, const QPolygonF& polygon);
    void add_new_slider(int index, const QPolygonF& polygon);
    const std::unordered_map<unsigned int, QPolygonF>& get_trace_map(size_t slide) const;

    size_t n_sliders() const noexcept { return sliders.size(); }

    const QString& get_slider_name(size_t id) const;

    QString get_unique_name(const QString& str) const;
    QString get_unique_name_except(const QString& str, int exceptId) const;

    void change_current_value(size_t sliderIndex, int value);
    void remove_slider(size_t sliderIndex) noexcept;
    void eat_other_sliders(SlideApplier* other, unsigned int skew);

    int get_slider_current_value(size_t sliderIndex) const;

private:
    std::vector<std::unique_ptr<CharacterTrace>> sliders;
};

