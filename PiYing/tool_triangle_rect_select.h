#pragma once

#include <memory>
#include <qpointf>

namespace piying::tool {
	namespace texture {

		class TriangleSelect;

		class TriangleRectSelect final
		{
		public:
			TriangleRectSelect();
			~TriangleRectSelect() = default;

			void draw();
			void click(const QPointF& mouse);
			void mouse_move(const QPointF& mouse);
			void release(const QPointF& mouse);
			void delete_element();
			void escape();
			void enter();

		private:
			std::unique_ptr<piying::tool::texture::TriangleSelect> chTriangleSelect;
			const bool edit_skelen{ false };
			QPointF rect;
			bool isDraw = false;
			bool _is_press{ false };
		};

	}
}