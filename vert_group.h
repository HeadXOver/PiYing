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
    unsigned int operator[](int index) const;

    size_t vert_size() const;

private:
    std::vector<unsigned int> verts;
};