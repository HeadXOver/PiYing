#pragma once

#include <map>
#include <memory>

class CharacterTraces;

class SlideApplier
{
public:
    SlideApplier();
    ~SlideApplier();

private:
    std::map<int, std::unique_ptr<CharacterTraces>> slide_map;
};

