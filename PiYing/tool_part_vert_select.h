#pragma once

#include <qpointf>
#include <memory>

namespace piying {

	class SelectedPoints;

	namespace tool {

		enum class HandleControlMode;
		
		namespace part {

			class VertSelect final
			{
			public:
				VertSelect();
				~VertSelect();

				void change_edit_mode_by_setting_last_pos(const QPointF& mouse);
				void click_select(const QPointF& mouse);
				void update_selected_to_draw();
				void draw_handle_and_selected();
				void move_handle(const QPointF& mouse);
				void append(unsigned int index);
				void clear();
				void affirm_handle();
				void delete_element();
				void escape();

				bool contains(unsigned int index) const noexcept;

				const QPointF& get_last_pos() const noexcept { return lastPos; }

				HandleControlMode edit_mode() const noexcept { return _edit_mode; }

			private:
				QPointF handleCenterPoint;
				QPointF dHandleCenterPoint;
				QPointF lastHandleCenterPoint;
				QPointF lastDHandleCenterPoint;

				QPointF lastPos;

				HandleControlMode _edit_mode;

				std::unique_ptr<SelectedPoints> selected_points;
			};

		}

	}
}