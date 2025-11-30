#include "vert_groups.h"

#include "vert_group.h"
#include <qlist>

VertGroups::VertGroups()
{
}

VertGroups::~VertGroups()
{
	for (VertGroup* group : _groups) delete group;
}

void VertGroups::add_one_group(const QList<unsigned int>& indices)
{
	_groups.push_back(new VertGroup(indices));
}

int VertGroups::group_num() const
{
	return _groups.size();
}

VertGroup* VertGroups::operator[](int index) const 
{
	return _groups[index];
}