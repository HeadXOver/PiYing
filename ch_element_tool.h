#pragma once

#include "enum_character_texture_tool_state.h"

#include <memory>

class PiYingGL;
class QPainter;
class QPointF;
struct ClickBehavior;
struct MouseMoveBehavior;
struct ReleaseBehavior;
struct EscapeBehavior;
struct DeleteElementBehavior;
struct EnterBehavior;
struct DrawBehavior;

struct GlVertReference;

class ChElementTool final
{
public:
	ChElementTool(int current, PiYingGL& pygl, CharacterToolState chToolState);
	~ChElementTool();

	void click(const QPointF& mouse);
	void release(const QPointF& mouse);
	void move(const QPointF& mouse);
	void draw();
	void escape();
	void delete_element();
	void enter();

	void construct_add_triangle();
	void construct_add_poly();
	void construct_add_round();
	void construct_rect_select();
	void construct_libre_select();
	void construct_add_vert_trace();
	void construct_rect_select_triangle();
	void construct_libre_select_triangle();

private:

	std::unique_ptr<ClickBehavior> clickBehavior;
	std::unique_ptr<MouseMoveBehavior> moveBehavior;
	std::unique_ptr<ReleaseBehavior> releaseBehavior;
	std::unique_ptr<EscapeBehavior> escapeBehavior;
	std::unique_ptr<DeleteElementBehavior> deleteBehavior;
	std::unique_ptr<EnterBehavior> enterBehavior;
	std::unique_ptr<DrawBehavior> drawBehavior;

	std::unique_ptr<GlVertReference> glVertReference;
};
