#pragma once

#include <memory>
#include <qpolygonf>

namespace piying::tool {
	namespace texture {

		class VertSelect;

		class VertLibreSelect final
		{
		public:
			VertLibreSelect();
			~VertLibreSelect() = default;

			void draw();
			void click(const QPointF& mouse);
			void escape();
			void delete_element();
			void mouse_move(const QPointF& mouse);
			void release(const QPointF& mouse);

		private:
			std::unique_ptr<piying::tool::texture::VertSelect> chElementSelect;
			QPolygonF polygon;
			bool drawing{ false };
			bool edit_skelen{ false };
			bool isPress{ false };
		};

	}
}
