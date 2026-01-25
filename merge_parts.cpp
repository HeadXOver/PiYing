#include "time_line_gl.h"

#include "part.h"
#include "global_objects.h"

#include <qmenu>
#include <qmessagebox>

void TimelineGl::ask_merge_parts()
{
	assert(_moving_select_part._index >= 0 && _moving_select_part._index < _showing_parts.size());
	assert(_part_cursor._index >= 0 && _part_cursor._index < _showing_parts.size());

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

	int ret = QMessageBox::question(
		this,
		tr("提示"),
		tr("是否保留原部件"),
		QMessageBox::Yes | QMessageBox::No,
		QMessageBox::Yes
	);

	if (ret == QMessageBox::Yes) {
		parts.push_back(new Part(*_showing_parts[_part_cursor._index], *_showing_parts[_moving_select_part._index]));
		update_showing_parts();
	}
	else {
		if (!_showing_parts[_moving_select_part._index]->eat_another_part(*_showing_parts[_part_cursor._index])) return;

		/// 释放内存
		parts.erase(parts.begin() + _showing_parts[_part_cursor._index]->_lay_index);
	}
}

void TimelineGl::part_layer_merge()
{
	int ret = QMessageBox::question(
		this,
		tr("提示"),
		tr("是否保留原部件？"),
		QMessageBox::Yes | QMessageBox::No,
		QMessageBox::Yes
	);

	const int childIndex = _showing_parts[_part_cursor._index]->_lay_index;
	const int parentIndex = _showing_parts[_moving_select_part._index]->_lay_index;

	if (ret == QMessageBox::Yes) {
		parts.insert(parts.begin() + childIndex, new Part(*parts[childIndex]));
	}
	parts[parentIndex]->add_child(parts[childIndex]);

	_part_cursor.set_cursor(parentIndex);
}
