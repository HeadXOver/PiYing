#include "time_line_gl.h"

#include "scale_trans.h"
#include "base_math.h"
#include "part.h"
#include "global_objects.h"

void TimelineGl::init_part_cursor()
{
	_part_cursor.set_cursor(0);
	_moving_select_part.set_cursor(0);
}

Part* TimelineGl::get_current_part() const
{
	if(parts.size() == 0) return nullptr;

	if (_part_cursor._index < 0) return nullptr;

	return parts[_part_cursor._index];
}

void TimelineGl::move_time_cursor(float mouse_x)
{
	const float half_width = width() / 2.f;
	time_cursor = cus::max(half_width * (1.f - _scale_trans->scale_lenth), mouse_x - _scale_trans->trans_x * half_width); ///< 确保非负
}

int TimelineGl::get_index_by_mouse(const QPoint& mouse) const
{
	const int x = mouse.x() * 5 / width();
	const int y = mouse.y() * 5 / (height() * _ratio);

	const int index = 5 * y + x;

	if(index >= parts.size() || index < 0) return -1;

	return index;
}
