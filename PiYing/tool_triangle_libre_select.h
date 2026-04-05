#pragma once

#include <memory>
#include <qpolygonf>

namespace piying::tool {
	namespace texture {

		class TriangleSelect;

		class TriangleLibreSelect final
		{
		public:
			TriangleLibreSelect();
			~TriangleLibreSelect() = default;

			void draw();
			void click(const QPointF& mouse);
			void mouse_move(const QPointF& mouse);
			void release(const QPointF& mouse);
			void escape();
			void delete_element();
			void enter();

		private:
			std::unique_ptr<piying::tool::texture::TriangleSelect> chTriangleSelect;
			QPolygonF polygon;
			bool drawing = false;
			bool edit_skelen{ false };
			bool isPress = false;
		};

	}
}