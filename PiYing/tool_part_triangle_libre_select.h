#pragma once

#include <memory>
#include <qpolygonf>

namespace piying::tool {
	namespace part {

		class TriangleSelect;

		class TriangleLibreSelect final
		{
		public:
			TriangleLibreSelect();
			~TriangleLibreSelect();

			void draw();
			void click(const QPointF& mouse);
			void mouse_move(const QPointF& mouse);
			void release(const QPointF& mouse);
			void escape();
			void delete_element();

		private:
			std::unique_ptr<piying::tool::part::TriangleSelect> chTriangleSelect;
			QPolygonF polygon;
			bool drawing = false;
			bool isPress = false;
		};

	}
}