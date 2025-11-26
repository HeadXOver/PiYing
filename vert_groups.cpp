#include "vert_groups.h"

#include "vert_group.h"

VertGroups::VertGroups()
{
}

VertGroups::~VertGroups()
{
	for (VertGroup* group : _groups) delete group;
}
