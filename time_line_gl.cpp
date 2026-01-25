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

void TimelineGl::update_is_draw_part()
{
	/// 计算当前需要绘制的部分，先初始化标记
	partIsDraw.assign(parts.size(), false);

	assert(_part_cursor._index >= 0 && _part_cursor._index < _showing_parts.size());

	_showing_parts[_part_cursor._index]->add_to_draw();
}

void TimelineGl::update_showing_parts()
{
	_showing_parts.clear();

	if (!_part_to_show) {
		for (int i = 0; i < parts.size(); i++) {
			if (parts[i]->is_root()) _showing_parts.push_back(parts[i]);
		}
	}
}

Part* TimelineGl::get_current_part() const
{
	if(_showing_parts.size() == 0) return nullptr;

	assert(_part_cursor._index >= 0 && _part_cursor._index < _showing_parts.size());

	return _showing_parts[_part_cursor._index];
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

	if(index >= _showing_parts.size() || index < 0) return -1;

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

	if (index >= _showing_parts.size()) {
		o_dragType = (int)_showing_parts.size();
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
	assert(from >= 0 && to >= 0);
	assert(from < parts.size() && to <= parts.size());

	if (from == to || from + 1 == to) return;

	if (to + 1 == from) {
		part_swap_by_showing_index(from, to);
		_part_cursor.set_cursor(to);
		return;
	}

	if (from + 2 == to) {
		part_swap_by_showing_index(from, from + 1);
		_part_cursor.set_cursor(from + 1);
		return;
	}

	int fromIndex = _showing_parts[from]->_lay_index;
	Part* tmp = parts[fromIndex];

	size_t toIndex = to == _showing_parts.size() ? parts.size() : _showing_parts[to]->_lay_index;

	if (from < to) {
		--toIndex;
		--to;
		std::move(parts.begin() + fromIndex + 1, parts.begin() + toIndex + 1, parts.begin() + fromIndex);     // 左移 [i+1..j]
		parts[toIndex] = tmp;        // 放入目标
	}
	else {
		std::move_backward(parts.begin() + toIndex, parts.begin() + fromIndex, parts.begin() + fromIndex + 1); // 右移 [j..i-1]
		parts[toIndex] = tmp;        // 放入目标
	}

	/// 先设置光标
	_part_cursor.set_cursor(to);

	/// 再把该列出的part添加到_showing_parts中
	update_showing_parts();
}