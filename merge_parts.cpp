#include "time_line_gl.h"

#include <qmenu>

void TimelineGl::merge_parts(int orin, int target)
{

	QMenu tempMenu(this);
	tempMenu.addAction(QString("%1, %2").arg(orin).arg(target));

	QAction* act = tempMenu.exec(QCursor::pos());
	if (!act) return;
}