#pragma once

#include <memory>
#include <qpointf>

enum class ToolHandleControlMode;

namespace piying::tool {
	namespace part {

		class Handle final
		{
		public:
			Handle();
			~Handle();

			void draw();
			void click(const QPointF& mouse);
			void mouse_move(const QPointF& mouse);
			void release(const QPointF& mouse);
			void delete_element();

		private:
			QPointF handleCenterPoint;
			QPointF dHandleCenterPoint;
			QPointF lastHandleCenterPoint;
			QPointF lastDHandleCenterPoint;

			QPointF _last_pos;
			bool _is_press{ false };
			ToolHandleControlMode _edit_mode;
		};

	}
}