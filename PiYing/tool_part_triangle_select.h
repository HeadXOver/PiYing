#pragma once

#include <qpointf>
#include <memory>

enum class ToolHandleControlMode;

namespace piying {

	class SelectedTriangle;

	namespace tool::part {

		class TriangleSelect final
		{
		public:
			TriangleSelect();
			~TriangleSelect();

			ToolHandleControlMode get_edit_mode() const noexcept { return editMode; }

			void escape();
			void deleteElement();
			void draw_handle_and_selected();
			void changeEditMode();
			void mouse_press(const QPointF& mouse);
			void moveHandle(const QPointF& mouse);
			void affirmHandle();
			void click_select(const QPointF& mouse);
			void update_selected_to_draw();

		public:
			std::unique_ptr<SelectedTriangle> selected_trangle;

			QPointF lastPos;

		private:
			QPointF _gl_handle_center;
			QPointF _widget_handle_center;
			QPointF lastHandleCenterPoint;
			QPointF lastDHandleCenterPoint;

			ToolHandleControlMode editMode;
		};

	}
}
