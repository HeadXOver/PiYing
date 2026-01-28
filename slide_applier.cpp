#include "slide_applier.h"

#include "character_trace.h"
#include "PiYing.h"
#include "global_objects.h"

SlideApplier::SlideApplier()
{
}

SlideApplier::SlideApplier(const SlideApplier& applier1, const SlideApplier& applier2, unsigned int skew)
{
    sliders.reserve(applier1.sliders.size() + applier2.sliders.size());
    
    for (int i = 0; i < applier1.sliders.size(); ++i) {
        sliders.push_back(std::make_unique<CharacterTrace>(*applier1.sliders[i]));
    }

    QString toName;
    for (int i = 0; i < applier2.sliders.size(); ++i) {
        toName = get_unique_name(applier2.sliders[i]->name());
        sliders.push_back(std::make_unique<CharacterTrace>(*applier2.sliders[i], skew));
        sliders.back()->set_name(toName);
    }
}

SlideApplier::SlideApplier(const SlideApplier& other)
{
    for (int i = 0; i < other.sliders.size(); ++i) {
        sliders.push_back(std::make_unique<CharacterTrace>(*other.sliders[i]));
    }
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
    sliders.push_back(std::make_unique<CharacterTrace>(index, polygon, get_unique_name("new slider")));
    PiYing::getInstance().update_part_slider();
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

void SlideApplier::remove_slider(int sliderIndex)
{
    sliders.erase(sliders.begin() + sliderIndex);
}

void SlideApplier::eat_other_sliders(SlideApplier* other, unsigned int skew)
{
    const unsigned int tempSize = (unsigned int)other->sliders.size();

    // 把 other 拼接到 this 的末尾，使用移动语义
    sliders.insert(sliders.end(),
        std::make_move_iterator(other->sliders.begin()),
        std::make_move_iterator(other->sliders.end()));

    // 清空 other（所有元素已被移走，只剩空壳）
    other->sliders.clear();

    for (unsigned int i = tempSize; i < sliders.size(); ++i) {
        sliders[i]->add_skew(skew);
    }
}

int SlideApplier::get_slider_current_value(int sliderIndex) const
{
    return sliders[sliderIndex]->get_current_slider_value();
}

QString SlideApplier::get_unique_name(const QString& str) const
{
    QString s = str;
    bool repeat = false;
    for (int i = 0;; i++) {
        if(i != 0) s = str + QString::number(i);
        repeat = false;
        for (int j = 0; j < sliders.size(); ++j)
            if (sliders[j]->name() == s) {
                repeat = true;
                break;
            }
        if (!repeat) return s;
    }

    return QString();
}

QString SlideApplier::get_unique_name_except(const QString& str, int exceptId) const
{
    QString s = str;
    bool repeat = false;
    for (int i = 0;; i++) {
        if (i != 0) s = str + QString::number(i);
        repeat = false;
        for (int j = 0; j < sliders.size(); ++j)
            if (j != exceptId && sliders[j]->name() == s) {
                repeat = true;
                break;
            }
        if (!repeat) return s;
    }

    return QString();
}