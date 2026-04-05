#pragma once

#include <memory>
#include <qpolygonf>

namespace piying::tool {
	namespace part {

		class VertSelect;

		class VertLibreSelect final
		{
		public:
			VertLibreSelect();
			~VertLibreSelect();

			void draw();
			void delete_element();
			void click(const QPointF& mouse);
			void escape();
			void mouse_move(const QPointF& mouse);
			void release(const QPointF& mouse);

		private:
			std::unique_ptr<piying::tool::part::VertSelect> _vert_select;
			QPolygonF polygon;
			bool drawing{ false };
			bool edit_skelen{ false };
			bool isPress{ false };
		};

	}
}