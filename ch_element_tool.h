#pragma once

#include "enum_character_texture_tool_state.h"

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
	void draw(QPainter* painter);
	void escape();
	void delete_element();
	void enter();

	void construct_add_triangle();
	void construct_add_poly();
	void construct_add_round();
	void construct_rect_select();
	void construct_libre_select();

private:

	ClickBehavior* clickBehavior = nullptr;
	MouseMoveBehavior* moveBehavior = nullptr;
	ReleaseBehavior* releaseBehavior = nullptr;
	EscapeBehavior* escapeBehavior = nullptr;
	DeleteElementBehavior* deleteBehavior = nullptr;
	EnterBehavior* enterBehavior = nullptr;
	DrawBehavior* drawBehavior = nullptr;

	GlVertReference* glVertReference = nullptr;
};
