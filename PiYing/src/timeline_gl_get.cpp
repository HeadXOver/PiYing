#include "time_line_gl.h"

void TimelineGl::set_to_timeline() noexcept
{
	_ui_type = spTimelineGL::UiType::Timeline;
}

void TimelineGl::set_to_part() noexcept
{
	_ui_type = spTimelineGL::UiType::Part;
}

float TimelineGl::x_map_to_gl(const float x) const
{
	return 2 * x / static_cast<float>(width()) - 1.f;
}