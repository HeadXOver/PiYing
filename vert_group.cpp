#include "vert_group.h"

VertGroup::VertGroup()
{
}

VertGroup::VertGroup(const QList<unsigned int>& indices)
{
	verts.reserve(indices.size());
	for (auto& index : indices) verts.push_back(index);
}

void VertGroup::add_group(const QList<unsigned int>& indices)
{
}
