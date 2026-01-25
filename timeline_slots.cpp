#include "time_line_gl.h"

#include "parts.h"
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

	parts->remove(_showing_parts[_part_cursor._index]->_lay_index);

	update_showing_parts();

	if (_showing_parts.empty()) {
		_part_cursor.set_cursor(-1);
	}
	else {
		_part_cursor.set_cursor(0);
	}
}

void TimelineGl::part_copy()
{
	assert(_part_cursor._index >= 0 && _part_cursor._index < _showing_parts.size());

	if (_part_copying) {
		delete _part_copying;
	}

	_part_copying = new Part(*_showing_parts[_part_cursor._index]);
}

void TimelineGl::part_paste()
{
	assert(_part_cursor._index >= 0 && _part_cursor._index < _showing_parts.size());

	if (!_part_copying) return;

	parts->insert(_showing_parts[_part_cursor._index]->_lay_index + 1, new Part(*_part_copying));

	update_showing_parts();
}

void TimelineGl::part_swap_by_showing_index(int from, int to)
{
	if (from == to) return;

	assert(from >= 0 && from < parts.size());
	assert(to >= 0 && to < parts.size());

	Part* tmp = _showing_parts[from];
	_showing_parts[from] = _showing_parts[to];
	_showing_parts[to] = tmp;

	parts->swap(tmp->_lay_index, _showing_parts[from]->_lay_index);
}
