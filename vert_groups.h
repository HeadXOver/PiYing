#pragma once

#include <vector>

class VertGroup;

class VertGroups
{
public:
    VertGroups();
    ~VertGroups();

private:
    std::vector<VertGroup*> _groups;
};