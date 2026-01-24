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

void TimelineGl::part_delete()
{
	if (_part_cursor._index < 0 || _part_cursor._index >= parts.size()) {
		return;
	}

	parts[_part_cursor._index]->release_buffers();
	parts.erase(parts.begin() + _part_cursor._index);

	if (parts.empty()) {
		_part_cursor.set_cursor(-1);
	}
	else {
		_part_cursor.set_cursor(0);
	}
}
