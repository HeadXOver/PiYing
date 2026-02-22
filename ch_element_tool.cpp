#include "ch_element_tool.h"

#include "piYing.h"
#include "piYingGL.h"

#include "add_triangle.h"
#include "add_ch_tex_poly.h"
#include "ch_element_select.h"
#include "ch_triangle_select.h"
#include "ch_element_rect_select.h"
#include "ch_element_libre_select.h"
#include "ch_triangle_rect_select.h"
#include "ch_triangle_libre_select.h"
#include "ch_element_add_round.h"
#include "ch_add_vert_trace.h"

#include "enum_character_texture_tool_state.h"

#include <qpointf>

void ChElementTool::construct_add_triangle()
{
	std::shared_ptr<AddTriangle> data = std::make_shared<AddTriangle>();
	clickBehavior = std::make_unique<AddTriangleClick>(data);
	escapeBehavior = std::make_unique<AddTriangleEscape>(data);
	drawBehavior = std::make_unique<AddTriangleDraw>(data);
	deleteBehavior = std::make_unique<AddTriangleDelete>(data);
}

void ChElementTool::construct_add_poly()
{
	std::shared_ptr<AddChTexPoly> data = std::make_shared<AddChTexPoly>();
	clickBehavior = std::make_unique<AddPolyClick>(data);
	escapeBehavior = std::make_unique<AddPolyEscape>(data);
	drawBehavior = std::make_unique<AddPolyDraw>(data);
	deleteBehavior = std::make_unique<AddPolyDelete>(data);
	enterBehavior = std::make_unique<AddPolyEnter>(data);
}

void ChElementTool::construct_add_round()
{
	std::shared_ptr<ChElementAddRound> data = std::make_shared<ChElementAddRound>();
	clickBehavior = std::make_unique<AddRoundClick>(data);
	moveBehavior = std::make_unique<AddRoundMove>(data);
	releaseBehavior = std::make_unique<AddRoundRelease>(data);
	drawBehavior = std::make_unique<AddRoundDraw>(data);
}

void ChElementTool::construct_rect_select()
{
	std::shared_ptr<ChElementRectSelect> data = std::make_shared<ChElementRectSelect>();
	clickBehavior = std::make_unique<RectSelectClick>(data);
	escapeBehavior = std::make_unique<RectSelectEscape>(data);
	drawBehavior = std::make_unique<RectSelectDraw>(data);
	releaseBehavior = std::make_unique<RectSelectRelease>(data);
	moveBehavior = std::make_unique<RectSelectMove>(data);
	deleteBehavior = std::make_unique<RectSelectDelete>(data);
}

void ChElementTool::construct_libre_select()
{
	std::shared_ptr<ChElementLibreSelect> data = std::make_shared<ChElementLibreSelect>();
	clickBehavior = std::make_unique<LibreSelectClick>(data);
	escapeBehavior = std::make_unique<LibreSelectEscape>(data);
	drawBehavior = std::make_unique<LibreSelectDraw>(data);
	releaseBehavior = std::make_unique<LibreSelectRelease>(data);
	moveBehavior = std::make_unique<LibreSelectMove>(data);
	deleteBehavior = std::make_unique<LibreSelectDelete>(data);
}

void ChElementTool::construct_add_vert_trace()
{
	std::shared_ptr<ChAddVertTrace> data = std::make_shared<ChAddVertTrace>();
	clickBehavior = std::make_unique<AddVertTraceClick>(data);
	drawBehavior = std::make_unique<AddVertTraceDraw>(data);
	releaseBehavior = std::make_unique<AddVertTraceRelease>(data);
	moveBehavior = std::make_unique<AddVertTraceMove>(data);
}

void ChElementTool::construct_rect_select_triangle()
{
	std::shared_ptr<ChTriangleRectSelect> data = std::make_shared<ChTriangleRectSelect>();
	clickBehavior = std::make_unique<RectSelectTriangleClick>(data);
	escapeBehavior = std::make_unique<RectSelectTriangleEscape>(data);
	drawBehavior = std::make_unique<RectSelectTriangleDraw>(data);
	releaseBehavior = std::make_unique<RectSelectTriangleRelease>(data);
	moveBehavior = std::make_unique<RectSelectTriangleMove>(data);
	deleteBehavior = std::make_unique<RectSelectTriangleDelete>(data);
	enterBehavior = std::make_unique<RectSelectTriangleEnter>(data);
}

void ChElementTool::construct_libre_select_triangle()
{
	std::shared_ptr<ChTriangleLibreSelect> data = std::make_shared<ChTriangleLibreSelect>();
	clickBehavior = std::make_unique<LibreSelectTriangleClick>(data);
	escapeBehavior = std::make_unique<LibreSelectTriangleEscape>(data);
	drawBehavior = std::make_unique<LibreSelectTriangleDraw>(data);
	releaseBehavior = std::make_unique<LibreSelectTriangleRelease>(data);
	moveBehavior = std::make_unique<LibreSelectTriangleMove>(data);
	deleteBehavior = std::make_unique<LibreSelectTriangleDelete>(data);
	enterBehavior = std::make_unique<LibreSelectTriangleEnter>(data);
}

ChElementTool::ChElementTool(CharacterToolState chToolState)
{
	switch (chToolState) {
	case CharacterToolState::None: return;
	case CharacterToolState::AddTriangle: construct_add_triangle(); break;
	case CharacterToolState::AddPoly: construct_add_poly(); break;
	case CharacterToolState::AddRound: construct_add_round(); break;
	case CharacterToolState::RectSelectVert: construct_rect_select(); break;
	case CharacterToolState::LibreSelectVert: construct_libre_select(); break;
	case CharacterToolState::AddVertTrace: construct_add_vert_trace(); break;
	case CharacterToolState::RectSelectTriangle: construct_rect_select_triangle(); break;
	case CharacterToolState::LibreSelectTriangle: construct_libre_select_triangle(); break;
	}
}

ChElementTool::~ChElementTool() {}

void ChElementTool::click(const QPointF& mouse) {
	if(clickBehavior) clickBehavior->click(mouse);
}

void ChElementTool::release(const QPointF& mouse)
{
	if (releaseBehavior) releaseBehavior->release(mouse);
}

void ChElementTool::move(const QPointF& mouse)
{
	if (moveBehavior) moveBehavior->mouse_move(mouse);
}

void ChElementTool::draw()
{
	if(drawBehavior) drawBehavior->draw();
}

void ChElementTool::escape()
{
	if(escapeBehavior) escapeBehavior->escape();
}

void ChElementTool::delete_element() 
{
	if (deleteBehavior) deleteBehavior->delete_element();
}

void ChElementTool::enter() 
{
	if (enterBehavior) enterBehavior->enter();
}
