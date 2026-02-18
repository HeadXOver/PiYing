#include "time_line_gl.h"

#include "scale_trans.h"
#include "base_math.h"
#include "parts.h"
#include "part.h"

#include <qmessagebox>

void TimelineGl::init_part_cursor()
{
	if (_part_cursor._index >= 0) {
		return;
	}

	_part_cursor.set_cursor(0);
	_moving_select_part.set_cursor(-1);
}

void TimelineGl::release_buffers(unsigned int vao, unsigned int vbo, unsigned int ebo) noexcept
{
	makeCurrent();	

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &ebo);
	doneCurrent();
}

void TimelineGl::update_showing_parts()
{
	if (!_part_to_show) {
		Parts::getInstance().output_root_to_show(_showing_parts);
		return;
	}

	Parts::getInstance().output_detail_to_show(_showing_parts, *_part_to_show);
}

void TimelineGl::update_is_draw_by_piying()
{
	Parts::getInstance().add_to_draw_by_piying();
}

Part* TimelineGl::get_current_part() const
{
	if(_showing_parts.size() == 0) return nullptr;

	return _showing_parts[_part_cursor._index];
}

void TimelineGl::move_time_cursor(float mouse_x)
{
	const float half_width = width() / 2.f;
	time_cursor = PiYingCus::max(half_width * (1.f - _scale_trans->scale_lenth), mouse_x - _scale_trans->trans_x * half_width); ///< 确保非负
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

int TimelineGl::current_layer() const
{
	if(_showing_parts.size() == 0) return -1;

	if (_part_cursor._index < 0) return -1;

	return _showing_parts[_part_cursor._index]->_lay_index;
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

	size_t toIndex;
	if (to == _showing_parts.size()) {
		toIndex = _showing_parts[to - 1]->_lay_index + 1;
	}
	else {
		toIndex = _showing_parts[to]->_lay_index;
	}

	Parts::getInstance().insert_from_to(_showing_parts[from]->_lay_index, toIndex);

	/// 先设置光标
	_part_cursor.set_cursor((from < to) ? to - 1 : to);
	update_showing_parts();
}