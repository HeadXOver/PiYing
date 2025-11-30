#pragma once

#include <vector>
#include <QList>

class VertGroup;

class VertGroups
{
public:
    VertGroups();
    ~VertGroups();

public:
    void add_one_group(const QList<unsigned int>& indices);

private:
    std::vector<VertGroup*> _groups;
};