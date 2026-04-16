#pragma once

#include <qpointf>

namespace piying::tool {
	namespace part {

		class AddLineTrace final
		{
		public:
			void click(const QPointF& mouse);
			void mouse_move(const QPointF& mouse);
			void release(const QPointF& mouse);
			void draw();

		private:
			int current_index = -1;
			QPointF _origin_vert_pos;
			QPointF _current_mouse_pos;
		};

	}
}
