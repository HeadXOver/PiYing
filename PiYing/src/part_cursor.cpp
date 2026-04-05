#include "time_line_gl.h"

#include "PiYingGL.h"

void spTimelineGL::PartCursor::set_cursor(int index) noexcept
{
	_index = index;

	x = -0.8f + (index % 5) * 0.4f;
	y = 0.8f - (index / 5) * 0.4f * TimelineGl::getInstance().ratio();

	PiYingGL::getInstance().update_ch_tool();
}

void spTimelineGL::PartCursor::update_after_resize() noexcept
{
	y = 0.8f - (_index / 5) * 0.4f * TimelineGl::getInstance().ratio();
}