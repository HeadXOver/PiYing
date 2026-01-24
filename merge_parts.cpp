#include "time_line_gl.h"

#include "part.h"
#include "global_objects.h"

#include <qmenu>
#include <qmessagebox>

void TimelineGl::ask_merge_parts()
{
	_merge_menu->exec(QCursor::pos());
}

void TimelineGl::part_beside_merge()
{
	if (!parts[_part_cursor._index]->same_texture_as(*parts[_moving_select_part._index])) {
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
		parts.push_back(std::make_shared<Part>(*parts[_part_cursor._index], *parts[_moving_select_part._index]));
	}
	else {
		if (!parts[_moving_select_part._index]->eat_another_part(*parts[_part_cursor._index])) return;

		/// 释放图形资源
		parts[_part_cursor._index]->release_buffers();

		/// 释放内存
		parts.erase(parts.begin() + _part_cursor._index);
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

	const int childIndex = _part_cursor._index;
	const int parentIndex = _moving_select_part._index;

	if (ret == QMessageBox::Yes) {
		parts.insert(parts.begin() + childIndex, std::make_shared<Part>(*parts[childIndex]));
	}
	parts[parentIndex]->add_child(parts[childIndex]);

	_part_cursor.set_cursor(parentIndex);
}
