#pragma once

#include <memory>
#include <qpointf>

namespace piying {
	
	namespace tool {

		enum class HandleControlMode;

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
				double _last_x;
				double _last_y;
				bool _is_press{ false };
				HandleControlMode _edit_mode;
			};

		}

	}
}