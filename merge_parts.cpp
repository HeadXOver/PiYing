#include "time_line_gl.h"

#include "part.h"
#include "global_objects.h"

#include <qmenu>
#include <qmessagebox>

void TimelineGl::ask_merge_parts()
{
	_merge_menu->exec(QCursor::pos());
}

void TimelineGl::part_beside_new()
{
	parts.push_back(std::make_shared<Part>(*parts[_part_cursor._index], *parts[_moving_select_part._index]));
}

void TimelineGl::part_beside_ref()
{
	if (!parts[_moving_select_part._index]->eat_another_part(*parts[_part_cursor._index])) return;

	/// 释放图形资源
	parts[_part_cursor._index]->release_buffers();

	/// 释放内存
	parts.erase(parts.begin() + _part_cursor._index);
}

void TimelineGl::part_layer_new()
{
	parts[_moving_select_part._index]->add_copied_child(parts[_part_cursor._index]);
}

void TimelineGl::part_layer_ref()
{
	parts[_moving_select_part._index]->add_child(parts[_part_cursor._index]);

	parts.erase(parts.begin() + _part_cursor._index);
}
