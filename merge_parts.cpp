#include "time_line_gl.h"

#include <qmenu>
#include <qmessagebox>

void TimelineGl::ask_merge_parts()
{
	_merge_menu->exec(QCursor::pos());
}

void TimelineGl::part_beside_new()
{
	QMessageBox::warning(this, "1", QString("%1, %2").arg(_part_cursor._index).arg(_moving_select_part._index));
}

void TimelineGl::part_beside_ref()
{
	QMessageBox::warning(this, "2", QString("%1, %2").arg(_part_cursor._index).arg(_moving_select_part._index));
}

void TimelineGl::part_layer_new()
{
	QMessageBox::warning(this, "3", QString("%1, %2").arg(_part_cursor._index).arg(_moving_select_part._index));
}

void TimelineGl::part_layer_ref()
{
	QMessageBox::warning(this, "4", QString("%1, %2").arg(_part_cursor._index).arg(_moving_select_part._index));
}
