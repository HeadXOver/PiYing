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
		Parts::getInstance().add(new Part(*_showing_parts[_part_cursor._index], *_showing_parts[_moving_select_part._index]));
	}
	else if (ans == QMessageBox::No) {
		if (!_showing_parts[_moving_select_part._index]->eat_another_part(*_showing_parts[_part_cursor._index])) return;

		/// 释放内存
		Parts::getInstance().remove(_showing_parts[_part_cursor._index]->_lay_index);
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
		Parts::getInstance().insert(childIndex, new Part(*(Parts::getInstance().get_part(childIndex))));
	}
	Parts::getInstance().get_part(parentIndex)->add_child(Parts::getInstance().get_part(childIndex));

	update_showing_parts();

	if (_part_cursor._index > _moving_select_part._index) {
		_part_cursor.set_cursor(_moving_select_part._index);
	}
	else {
		_part_cursor.set_cursor(_moving_select_part._index - 1);
	}

	update_is_draw_by_piying();
}
