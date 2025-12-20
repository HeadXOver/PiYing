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
    ~SlideApplier();

    bool add_trace2(int id, int index, const QPolygonF& polygon);
    const std::map<int, std::unique_ptr<CharacterTrace>>& get_traces(int id);
    void remove_slider_by_id(int id);

    bool add_trace(int sliderId, int index, const QPolygonF& polygon);
    const std::map<unsigned int, QPolygonF>& get_trace_map(int slide) const;

    unsigned int n_sliders() const;

    const QString& get_slider_name(int id) const;

private:
    std::vector<std::unique_ptr<CharacterTrace>> sliders;

    std::map<int, std::map<int, std::unique_ptr<CharacterTrace>>> slide_map;
};

