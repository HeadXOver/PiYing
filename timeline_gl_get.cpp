#include "time_line_gl.h"

void TimelineGl::set_to_timeline()
{
	_ui_type = UiType::Timeline;
}

void TimelineGl::set_to_part()
{
	_ui_type = UiType::Part;
}

float TimelineGl::x_map_to_gl(const float x) const
{
	return x / width() * 2.f - 1.f;
}