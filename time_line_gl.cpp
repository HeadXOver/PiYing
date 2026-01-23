#include "time_line_gl.h"

#include "scale_trans.h"
#include "base_math.h"
#include "part.h"
#include "global_objects.h"

#include <qmessagebox>

void TimelineGl::init_part_cursor()
{
	_part_cursor.set_cursor(0);
	_moving_select_part.set_cursor(0);
}

void TimelineGl::release_buffers(unsigned int vao, unsigned int vbo, unsigned int ebo)
{
	makeCurrent();
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &ebo);
	doneCurrent();
}

std::shared_ptr<Part> TimelineGl::get_current_part() const
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
	constexpr float partLaySpace = 1.f - spTimelineGL::scroll_width;
	const float fx = mouse.x() * 5 / (width() * partLaySpace);
	const int y = mouse.y() * 5 / (height() * _ratio) + _scroll_positon * _part_total_scale * 5 / _ratio;

	const float x = floor(fx);

	if(x < 0 || x > 4) return -1;

	const int index = 5 * y + x;

	if(index >= parts.size() || index < 0) return -1;

	return index;
}

int TimelineGl::get_index_by_mouse(const QPoint& mouse, int& o_dragType) const
{
	constexpr float partLaySpace = 1.f - spTimelineGL::scroll_width;
	const float fx = mouse.x() * 5 / (width() * partLaySpace);
	const int y = mouse.y() * 5 / (height() * _ratio) + _scroll_positon * _part_total_scale * 5 / _ratio;

	const float x = floor(fx);
	const float r = fx - x;

	const int index = 5 * y + x;

	if (index < 0) {
		o_dragType = -1;
		return -1;
	}

	if (index >= parts.size()) {
		o_dragType = (int)parts.size();
		return -1;
	}

	if (r < 0.1f) {
		o_dragType = index;
	}
	else if (r > 0.9f) {
		o_dragType = index + 1;
	}
	else {
		o_dragType = -1;
	}

	return index;
}

void TimelineGl::insert_from_to(int from, int to)
{
	if (from < 0 || to < 0 || from >= parts.size() || to > parts.size()) return;

	if (from == to || from + 1 == to) return;

	if (to + 1 == from) {
		std::swap(parts[from], parts[to]);
		_part_cursor.set_cursor(to);
		return;
	}

	if (from + 2 == to) {
		std::swap(parts[from], parts[from + 1]);
		_part_cursor.set_cursor(from + 1);
		return;
	}

	/// 交换
	auto tmp = std::move(parts[from]);

	if (from < to) {
		--to;
		std::move(parts.begin() + from + 1, parts.begin() + to + 1, parts.begin() + from);     // 左移 [i+1..j]
		parts[to] = std::move(tmp);        // 放入目标
	}
	else {
		std::move_backward(parts.begin() + to, parts.begin() + from, parts.begin() + from + 1); // 右移 [j..i-1]
		parts[to] = std::move(tmp);        // 放入目标
	}

	_part_cursor.set_cursor(to);
}