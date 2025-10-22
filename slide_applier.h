#pragma once

#include <map>
#include <memory>

class CharacterTrace;

class SlideApplier
{
public:
    SlideApplier();
    ~SlideApplier();

private:
    std::map<int, std::map<int, std::unique_ptr<CharacterTrace>>> slide_map;
};

