#include "time_line_gl.h"

#include "global_objects.h"

void spTimelineGL::PartCursor::set_cursor(int index)
{
	_index = index;

	x = -0.8f + (index % 5) * 0.4f;
	y = 0.8f - (index / 5) * 0.4f * TimelineGl::getInstance().ratio();
}

void spTimelineGL::PartCursor::update_after_resize()
{
	y = 0.8f - (_index / 5) * 0.4f * TimelineGl::getInstance().ratio();
}