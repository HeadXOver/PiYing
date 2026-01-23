#include "time_line_gl.h"

#include "part.h"
#include "piYingGL.h"
#include "PiYing.h"
#include "global_objects.h"

void TimelineGl::part_exchange()
{
	std::swap(parts[_part_cursor._index], parts[_moving_select_part._index]);

	piYing->update_part_slider();
	piYingGL->update();
}