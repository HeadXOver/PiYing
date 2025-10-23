#include "slide_applier.h"

#include "character_trace.h"

SlideApplier::SlideApplier()
{
}

SlideApplier::~SlideApplier()
{
}

bool SlideApplier::add_trace(int id, int index, const QPolygonF& polygon)
{
    std::map<int, std::unique_ptr<CharacterTrace>>& traces = slide_map[id];

    if(traces.count(index)) return false;

    traces[index] = std::make_unique<CharacterTrace>(polygon);

    return true;
}

const std::map<int, std::unique_ptr<CharacterTrace>>& SlideApplier::get_traces(int id)
{
    return slide_map[id];
}

void SlideApplier::remove_slider_by_id(int id)
{
    slide_map.erase(id);
}
