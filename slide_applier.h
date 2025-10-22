#pragma once

#include <map>
#include <memory>

class CharacterTrace;
class QPolygonF;

class SlideApplier
{
public:
    SlideApplier();
    ~SlideApplier();

    bool add_trace(int id, int index, const QPolygonF& polygon);

private:
    std::map<int, std::map<int, std::unique_ptr<CharacterTrace>>> slide_map;
};

