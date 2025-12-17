#include "time_line_gl.h"

#include "KeyboardStateWin.h"
#include "scale_trans.h"
#include "base_math.h"
#include "part.h"
#include "piYingGL.h"
#include "PiYing.h"
#include "global_objects.h"

#include <qmouseevent>

void TimelineGl::wheelEvent(QWheelEvent* ev)
{
	if (_ui_type == UiType::Timeline) {
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
	else if (_ui_type == UiType::Part) {

	}
	update();
	ev->accept();
}

void TimelineGl::mousePressEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::MiddleButton) {
		lastMiddleButtonPos = event->position();
		*_last_scale_trans = *_scale_trans;
	}
	else if (event->buttons() == Qt::LeftButton) {
		_pressing = true;

		if (_ui_type == UiType::Timeline) {
			if (KeyboardStateWin::isCtrlHeld()) {
				_draging_cursor = true;
				move_time_cursor(event->position().x());
				update();
				return;
			}
			const float diff_map = event->position().x() - (width() * _scale_trans->trans_x / 2.f + time_cursor);
			const float diff_gl = diff_map * 2.f / (float)width();
			if (cus::abs(diff_gl) < 0.02f) {
				_draging_cursor = true;
			}
		}
		else if (_ui_type == UiType::Part) {
			const int index = get_index_by_mouse(event->pos());
			if (index < 0) return;

			parts[index]->update_scale();

			if (index != _part_cursor._index) {
				_part_cursor.set_cursor(index);
				_moving_select_part.set_cursor(index);

				piYing->update_part_slider();
				piYingGL->update();
			}

			update();
		}
	}
}

void TimelineGl::mouseReleaseEvent(QMouseEvent* event)
{
	_pressing = false;
	_draging_cursor = false;

	if (_ui_type == UiType::Part) {
		const int index = get_index_by_mouse(event->pos());
		if (index < 0) return;

		parts[index]->update_scale();

		if (index != _part_cursor._index) {
			merge_parts(_part_cursor._index, index);

			_part_cursor.set_cursor(index);

			piYing->update_part_slider();
			piYingGL->update();
		}
	}
	update();
}

void TimelineGl::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton) {
		if (_ui_type == UiType::Timeline) {
			if (_draging_cursor || KeyboardStateWin::isCtrlHeld()) {
				move_time_cursor(event->position().x());
				update();
			}
		}
		else if (_pressing && _ui_type == UiType::Part) {
			const int index = get_index_by_mouse(event->pos());
			_moving_select_part.set_cursor(index);
			update();
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