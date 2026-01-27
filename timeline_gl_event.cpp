#include "time_line_gl.h"

#include "KeyboardStateWin.h"
#include "scale_trans.h"
#include "base_math.h"
#include "parts.h"
#include "part.h"
#include "piYingGL.h"
#include "PiYing.h"
#include "global_objects.h"

#include <qmouseevent>
#include <qmenu>
#include <qmessagebox>

void TimelineGl::wheelEvent(QWheelEvent* ev)
{
	if (_ui_type == spTimelineGL::UiType::Timeline) {
		float scaleFactor = 1.0f + ev->angleDelta().y() / 1200.f;
		if (scaleFactor < 0.1f) scaleFactor = 0.1f;

		_scale_trans->scale_lenth *= scaleFactor;

		const float gl_mouse_x = x_map_to_gl(ev->position().x());
		const float diff_x = gl_mouse_x - _scale_trans->trans_x;
		const float to_trans_x = diff_x * (1.f - scaleFactor);

		_scale_trans->trans_x = cus::min(_scale_trans->scale_lenth - 1.f, _scale_trans->trans_x + to_trans_x);

		const float xTimelineCursor = width() * _scale_trans->trans_x / 2.f + time_cursor;
		const float diffTimelineCursor = xTimelineCursor - width() * (_scale_trans->trans_x + 1.f) / 2.f;
		time_cursor -= diffTimelineCursor * (1.f - scaleFactor);

		*_last_scale_trans = *_scale_trans;
	}
	else if (_ui_type == spTimelineGL::UiType::Part) {
		const float scaleTrans = ev->angleDelta().y() / 1200.f;

		const float toSet = _scroll_positon - scaleTrans;

		if (toSet < 0) {
			_scroll_positon = 0;
		}
		else if (toSet > 1.f - 1.f / _part_total_scale) {
			_scroll_positon = 1.f - 1.f / _part_total_scale;
		}
		else {
			_scroll_positon = toSet;
		}
	}
	update();
	ev->accept();
}

void TimelineGl::mousePressEvent(QMouseEvent* event)
{
	_moving_select_part.set_cursor(-1);

	if (event->button() == Qt::MiddleButton) {
		lastMiddleButtonPos = event->position();
		*_last_scale_trans = *_scale_trans;

		update();
		return;
	}

	if (event->button() == Qt::LeftButton) {
		_pressing = true;
		_last_mouse_pos = event->position();

		if (_ui_type == spTimelineGL::UiType::Timeline) {
			if (KeyboardStateWin::isCtrlHeld()) {
				_drag_type = spTimelineGL::DragType::Cursor;
				move_time_cursor(event->position().x());
				update();
				return;
			}
			const float diff_map = event->position().x() - (width() * _scale_trans->trans_x / 2.f + time_cursor);
			const float diff_gl = diff_map * 2.f / (float)width();
			if (cus::abs(diff_gl) < 0.02f) {
				_drag_type = spTimelineGL::DragType::Cursor;
			}
		}
		else if (_ui_type == spTimelineGL::UiType::Part) {
			if (event->pos().x() >= (1.f - spTimelineGL::scroll_width) * width() 
				&& event->pos().y() > height() * _scroll_positon 
				&& event->pos().y() < height() * (_scroll_positon + 1.f / _part_total_scale)
				) {
				_drag_type = spTimelineGL::DragType::Scroll;
				return;
			}

			const int index = get_index_by_mouse(event->pos());
			if (index < 0) return;

			_drag_type = spTimelineGL::DragType::Part;

			parts->update_scale(_showing_parts[index]->_lay_index);

			if (index != _part_cursor._index) {
				_part_cursor.set_cursor(index);
				_moving_select_part.set_cursor(index);

				piYing->update_part_slider();
				piYingGL->update();
			}
		}

		update();
		return;
	}

	if (event->button() == Qt::RightButton) {
		if (_ui_type == spTimelineGL::UiType::Part) {
			const int index = get_index_by_mouse(event->pos());

			if (index >= 0) {
				parts->update_scale(index);

				if (index != _part_cursor._index) {
					_part_cursor.set_cursor(index);
				}
			}

			piYing->update_part_slider();
			piYingGL->update();
			update();

			_part_menu->exec(QCursor::pos());

			piYing->update_part_slider();
			piYingGL->update();
			update();
			return;
		}
	}
}

void TimelineGl::mouseReleaseEvent(QMouseEvent* event)
{
	_pressing = false;

	if (_ui_type == spTimelineGL::UiType::Part && event->button() == Qt::LeftButton && _drag_type == spTimelineGL::DragType::Part) {
		const int index = get_index_by_mouse(event->pos(), _insert_part_index);

		const bool indexInRange = (index >= 0 && index < _showing_parts.size());

		if (_insert_part_index >= 0) {
			/// 插入部件
			insert_from_to(_part_cursor._index, _insert_part_index);
		}
		else if (index != _part_cursor._index && indexInRange) {
			/// 打开部件菜单
			ask_merge_parts();
		}
	}

	if (_part_cursor._index >= 0) parts->update_scale(_part_cursor._index);

	_insert_part_index = -1;

	_last_scroll_positon = _scroll_positon;

	_drag_type = spTimelineGL::DragType::None;

	piYing->update_part_slider();
	piYingGL->update();
	update();
}

void TimelineGl::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton) {
		if (_ui_type == spTimelineGL::UiType::Timeline) {
			if (_drag_type == spTimelineGL::DragType::Cursor || KeyboardStateWin::isCtrlHeld()) {
				move_time_cursor(event->position().x());
				update();
			}
		}
		else if (_pressing && _ui_type == spTimelineGL::UiType::Part) {
			switch (_drag_type) {
			case spTimelineGL::DragType::Part: {
				_insert_part_index = -1;
				_moving_select_part.set_cursor(
					get_index_by_mouse(event->pos(), _insert_part_index)
				);
				update();
			}break;
			case spTimelineGL::DragType::Scroll: {
				float toSet = _last_scroll_positon + (event->position().y() - _last_mouse_pos.y()) / height();
				const float maxLenth = 1.f - 1.f / _part_total_scale;

				if(toSet < 0) toSet = 0;
				else if (toSet > maxLenth) toSet = maxLenth;

				_scroll_positon = toSet;
				update();
			}break;
			}
		}
	}
	else if (event->buttons() == Qt::MiddleButton) {
		const float to_trans_x = _last_scale_trans->trans_x + (event->position().x() - lastMiddleButtonPos.x()) * 2.f / width();
		_scale_trans->trans_x = cus::min(_scale_trans->scale_lenth - 1.f, to_trans_x);

		size_t timeline_size = _timelines.size();
		if (timeline_size > 5) {
			float to_trans_y = _last_scale_trans->trans_y - (event->position().y() - lastMiddleButtonPos.y()) * 2.f / height();
			to_trans_y = cus::max(_scale_trans->scale_height - 1.f, to_trans_y);
			_scale_trans->trans_y = cus::min(0.4f * timeline_size - 2.f, to_trans_y);
		}
		update();
	}
}