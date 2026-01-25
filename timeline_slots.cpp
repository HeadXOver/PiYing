#include "time_line_gl.h"

#include "part.h"
#include "piYingGL.h"
#include "PiYing.h"
#include "global_objects.h"

void TimelineGl::part_exchange()
{
	part_swap_by_showing_index(_part_cursor._index, _moving_select_part._index);
}

void TimelineGl::part_delete()
{
	assert(_part_cursor._index >= 0 && _part_cursor._index < parts.size());

	parts[_part_cursor._index]->release_buffers();
	parts.erase(parts.begin() + _part_cursor._index);

	if (parts.empty()) {
		_part_cursor.set_cursor(-1);
	}
	else {
		_part_cursor.set_cursor(0);
	}
}

void TimelineGl::part_swap_by_showing_index(int from, int to)
{
	if (from == to) return;

	assert(from >= 0 && from < parts.size());
	assert(to >= 0 && to < parts.size());

	Part* fromPart = _showing_parts[from];
	Part* toPart = _showing_parts[to];

	_showing_parts[from] = toPart;
	_showing_parts[to] = fromPart;

	int indexFrom = fromPart->index;
	int indexTo = toPart->index;

	fromPart->index = indexTo;
	toPart->index = indexFrom;

	parts[indexFrom] = toPart;
	parts[indexTo] = fromPart;

	bool tmpBool = partIsDraw[indexFrom];
	partIsDraw[indexFrom] = partIsDraw[indexTo];
	partIsDraw[indexTo] = tmpBool;

	piYing->update_part_slider();
	piYingGL->update();
	update();
}
