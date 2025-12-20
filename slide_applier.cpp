#include "slide_applier.h"

#include "character_trace.h"
#include "PiYing.h"
#include "global_objects.h"

SlideApplier::SlideApplier()
{
}

SlideApplier::~SlideApplier()
{
}

void SlideApplier::remove_slider_by_id(int id)
{
    sliders.erase(sliders.begin() + id);
}

bool SlideApplier::add_trace_on_exist_slider(int sliderId, int index, const QPolygonF& polygon)
{
    CharacterTrace& trace = *sliders[sliderId];

    if (trace.have_point(index)) return false;
    trace.add_point(index, polygon);

    return true;
}

void SlideApplier::add_new_slider(int index, const QPolygonF& polygon)
{
    sliders.push_back(std::make_unique<CharacterTrace>(index, polygon, "new slider"));
    piYing->update_part_slider();
}

const std::map<unsigned int, QPolygonF>& SlideApplier::get_trace_map(int slide) const
{
    return sliders[slide]->get_traces();
}

unsigned int SlideApplier::n_sliders() const
{
    return (unsigned int)sliders.size();
}

const QString& SlideApplier::get_slider_name(int id) const
{
    return sliders[id]->name();
}

void SlideApplier::change_current_value(int sliderIndex, int value)
{
    sliders[sliderIndex]->set_current_value(value);
}

int SlideApplier::get_slider_current_value(int sliderIndex) const
{
    return sliders[sliderIndex]->get_current_slider_value();
}
