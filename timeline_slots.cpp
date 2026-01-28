#include "time_line_gl.h"

#include "parts.h"
#include "part.h"
#include "piYingGL.h"
#include "PiYing.h"
#include "cus_ask.h"

void TimelineGl::part_exchange()
{
	part_swap_by_showing_index(_part_cursor._index, _moving_select_part._index);

	_part_cursor.set_cursor(_moving_select_part._index);

	update_is_draw_by_piying();
}

void TimelineGl::part_delete()
{
	Part* toDelete = _showing_parts[_part_cursor._index];

	if (toDelete->have_child()) {
		switch (CUS_YES_NO_OR_CANCEL("是否保留子项？")) {
		case QMessageBox::Yes: {
		}break;
		case QMessageBox::No: {
			Parts::getInstance().remove_with_children(toDelete->_lay_index);
		}break;
		case QMessageBox::Cancel: return;
		}
	}
	else {
		if (!CUS_YES_OR_NO("是否删除？")) return;

		Parts::getInstance().remove(toDelete->_lay_index);
	}

	update_showing_parts();

	if (_showing_parts.empty()) {
		_part_cursor.set_cursor(-1);
	}
	else {
		_part_cursor.set_cursor(0);
	}

	update_is_draw_by_piying();
}

void TimelineGl::part_copy()
{
	if (_part_copying) {
		delete _part_copying;
		_part_copying = nullptr;
	}

	_part_copying = new Part(*_showing_parts[_part_cursor._index]);
}

void TimelineGl::part_paste()
{
	if (!_part_copying) return;

	Parts::getInstance().insert(_showing_parts[_part_cursor._index]->_lay_index + 1, new Part(*_part_copying));

	update_showing_parts();

	_part_cursor.set_cursor(_part_cursor._index + 1);

	update_is_draw_by_piying();
}

void TimelineGl::part_swap_by_showing_index(int from, int to)
{
	if (from == to) return;

	assert(from >= 0 && from < parts.size());
	assert(to >= 0 && to < parts.size());

	Part* tmp = _showing_parts[from];
	_showing_parts[from] = _showing_parts[to];
	_showing_parts[to] = tmp;

	Parts::getInstance().swap(tmp->_lay_index, _showing_parts[from]->_lay_index);
}
