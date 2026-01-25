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

	parts.erase(parts.begin() + _part_cursor._index);

	if (parts.empty()) {
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

	int pastePositionIndex = _showing_parts[_part_cursor._index]->_lay_index;

	if (pastePositionIndex == parts.size() - 1) {
		parts.push_back(new Part(*_part_copying));
	}
	else {
		parts.push_back(nullptr);
		std::copy(parts.begin() + pastePositionIndex + 1, parts.end(), parts.begin() + pastePositionIndex + 2);
		parts[pastePositionIndex + 1] = new Part(*_part_copying);
	}

	update_showing_parts();
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

	int indexFrom = fromPart->_lay_index;
	int indexTo = toPart->_lay_index;

	fromPart->_lay_index = indexTo;
	toPart->_lay_index = indexFrom;

	parts[indexFrom] = toPart;
	parts[indexTo] = fromPart;

	bool tmpBool = partIsDraw[indexFrom];
	partIsDraw[indexFrom] = partIsDraw[indexTo];
	partIsDraw[indexTo] = tmpBool;
}
