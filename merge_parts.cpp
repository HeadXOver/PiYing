#include "time_line_gl.h"

#include "part.h"
#include "parts.h"
#include "global_objects.h"
#include "cus_ask.h"

#include <qmenu>

void TimelineGl::ask_merge_parts()
{
	if (_showing_parts[_part_cursor._index]->same_texture_as(_showing_parts[_moving_select_part._index])) {
		_drag_same_texture_menu->exec(QCursor::pos());
	}
	else {
		_drag_diff_texture_menu->exec(QCursor::pos());
	}
}

void TimelineGl::part_beside_merge()
{
	if (!_showing_parts[_part_cursor._index]->same_texture_as(_showing_parts[_moving_select_part._index])) {
		QMessageBox::information(this, tr("提示"), tr("两个部件的纹理不同，无法合并！"));
		return;
	}

	QMessageBox::StandardButton ans = CUS_YES_NO_OR_CANCEL("是否保留原部件？");

	if (ans == QMessageBox::Yes) {
		parts->add(new Part(*_showing_parts[_part_cursor._index], *_showing_parts[_moving_select_part._index]));
	}
	else if (ans == QMessageBox::No) {
		if (!_showing_parts[_moving_select_part._index]->eat_another_part(*_showing_parts[_part_cursor._index])) return;

		/// 释放内存
		parts->remove(_showing_parts[_part_cursor._index]->_lay_index);
	}
	else {
		return;
	}

	update_showing_parts();

	if (_part_cursor._index > _moving_select_part._index) {
		_part_cursor.set_cursor(_moving_select_part._index);
	}
	else {
		_part_cursor.set_cursor(_moving_select_part._index - 1);
	}

	update_is_draw_by_piying();
}

void TimelineGl::part_layer_merge()
{
	QMessageBox::StandardButton ans = CUS_YES_NO_OR_CANCEL("是否保留原部件？");

	if (ans == QMessageBox::Cancel) {
		return;
	}

	const int childIndex = _showing_parts[_part_cursor._index]->_lay_index;
	const int parentIndex = _showing_parts[_moving_select_part._index]->_lay_index;

	if (ans== QMessageBox::Yes) {
		parts->insert(childIndex, new Part(*(parts->get_part(childIndex))));
	}
	parts->get_part(parentIndex)->add_child(parts->get_part(childIndex));

	update_showing_parts();

	if (_part_cursor._index > _moving_select_part._index) {
		_part_cursor.set_cursor(_moving_select_part._index);
	}
	else {
		_part_cursor.set_cursor(_moving_select_part._index - 1);
	}

	update_is_draw_by_piying();
}
