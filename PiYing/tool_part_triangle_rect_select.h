#pragma once

#include <memory>
#include <qpointf>

namespace piying::tool {
	namespace part {

		class TriangleSelect;

		class TriangleRectSelect final
		{
		public:
			TriangleRectSelect();
			~TriangleRectSelect();

			void draw();
			void click(const QPointF& mouse);
			void mouse_move(const QPointF& mouse);
			void release(const QPointF& mouse);
			void delete_element();
			void escape();

		private:
			std::unique_ptr<piying::tool::part::TriangleSelect> chTriangleSelect;
			QPointF rect;
			bool isDraw = false;
			bool _is_press{ false };
		};

	}
}