#pragma once

#include <memory>
#include <qpointf>

namespace piying::tool {
	namespace part {

		class VertSelect;

		class VertRectSelect final
		{
		public:
			VertRectSelect();
			~VertRectSelect();

			void draw();
			void delete_element();
			void click(const QPointF& mouse);
			void escape();
			void mouse_move(const QPointF& mouse);
			void release(const QPointF& mouse);

		private:
			std::unique_ptr<VertSelect> _vert_select;
			double rectX{};
			double rectY{};
			bool isDraw{ false };
			bool _is_press{ false };
		};

	}
}