#pragma once

#include <vector>
#include <qlist>

class VertGroup
{
public:
    VertGroup();
    VertGroup(const QList<unsigned int>& indices);

public:
    void add_group(const QList<unsigned int>& indices);

private:
    std::vector<unsigned int> verts;
};