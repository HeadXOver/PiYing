#pragma once

#include <memory>

class QPointF;

enum class CharacterToolState;

namespace piying::tool {

	struct ActBehavior;
	struct PosBehavior;

	class PiyingTool final
	{
	public:
		PiyingTool(CharacterToolState chToolState);
		~PiyingTool();

		void click(const QPointF& mouse);
		void release(const QPointF& mouse);
		void move(const QPointF& mouse);
		void draw();
		void escape();
		void delete_element();
		void enter();

	private:
		void construct_void();
		void construct_add_triangle();
		void construct_add_poly();
		void construct_add_round();
		void construct_rect_select();
		void construct_libre_select();
		void construct_add_vert_trace();
		void construct_add_line_trace();
		void construct_rect_select_triangle();
		void construct_libre_select_triangle();
		void construct_libre_select_vert_part();
		void construct_rect_select_vert_part();
		void construct_libre_select_triangle_part();
		void construct_rect_select_triangle_part();
		void construct_part_handle();

	private:

		std::unique_ptr<PosBehavior> clickBehavior;
		std::unique_ptr<PosBehavior> moveBehavior;
		std::unique_ptr<PosBehavior> releaseBehavior;
		std::unique_ptr<ActBehavior> escapeBehavior;
		std::unique_ptr<ActBehavior> deleteBehavior;
		std::unique_ptr<ActBehavior> enterBehavior;
		std::unique_ptr<ActBehavior> drawBehavior;
	};

}