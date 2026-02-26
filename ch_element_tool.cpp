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

	PIYING_CLICK_BEHAVIOR(AddTriangle);
	PIYING_ESCAPE_BEHAVIOR(AddTriangle);
	PIYING_DELETE_BEHAVIOR(AddTriangle);
	PIYING_DRAW_BEHAVIOR(AddTriangle);

	clickBehavior = std::make_unique<AddTriangleClick>(data);
	escapeBehavior = std::make_unique<AddTriangleEscape>(data);
	deleteBehavior = std::make_unique<AddTriangleDelete>(data);
	drawBehavior = std::make_unique<AddTriangleDraw>(data);
}

void ChElementTool::construct_add_poly()
{
	std::shared_ptr<AddChTexPoly> data = std::make_shared<AddChTexPoly>();

	PIYING_ESCAPE_BEHAVIOR(AddChTexPoly);
	PIYING_DELETE_BEHAVIOR(AddChTexPoly);
	PIYING_CLICK_BEHAVIOR(AddChTexPoly);
	PIYING_DRAW_BEHAVIOR(AddChTexPoly);
	PIYING_ENTER_BEHAVIOR(AddChTexPoly);

	clickBehavior = std::make_unique<AddChTexPolyClick>(data);
	escapeBehavior = std::make_unique<AddChTexPolyEscape>(data);
	drawBehavior = std::make_unique<AddChTexPolyDraw>(data);
	deleteBehavior = std::make_unique<AddChTexPolyDelete>(data);
	enterBehavior = std::make_unique<AddChTexPolyEnter>(data);
}

void ChElementTool::construct_add_round()
{
	std::shared_ptr<ChElementAddRound> data = std::make_shared<ChElementAddRound>();

	PIYING_CLICK_BEHAVIOR(ChElementAddRound);
	PIYING_MOVEMOUSE_BEHAVIOR(ChElementAddRound);
	PIYING_RELEASE_BEHAVIOR(ChElementAddRound);
	PIYING_DRAW_BEHAVIOR(ChElementAddRound);

	clickBehavior = std::make_unique<ChElementAddRoundClick>(data);
	moveBehavior = std::make_unique<ChElementAddRoundMoveMouse>(data);
	releaseBehavior = std::make_unique<ChElementAddRoundRelease>(data);
	drawBehavior = std::make_unique<ChElementAddRoundDraw>(data);
}

void ChElementTool::construct_rect_select()
{
	std::shared_ptr<ChElementRectSelect> data = std::make_shared<ChElementRectSelect>();

	PIYING_CLICK_BEHAVIOR(ChElementRectSelect);
	PIYING_MOVEMOUSE_BEHAVIOR(ChElementRectSelect);
	PIYING_RELEASE_BEHAVIOR(ChElementRectSelect);
	PIYING_DELETE_BEHAVIOR(ChElementRectSelect);
	PIYING_DRAW_BEHAVIOR(ChElementRectSelect);
	PIYING_ESCAPE_BEHAVIOR(ChElementRectSelect);

	clickBehavior = std::make_unique<ChElementRectSelectClick>(data);
	escapeBehavior = std::make_unique<ChElementRectSelectEscape>(data);
	drawBehavior = std::make_unique<ChElementRectSelectDraw>(data);
	releaseBehavior = std::make_unique<ChElementRectSelectRelease>(data);
	moveBehavior = std::make_unique<ChElementRectSelectMoveMouse>(data);
	deleteBehavior = std::make_unique<ChElementRectSelectDelete>(data);
}

void ChElementTool::construct_libre_select()
{
	std::shared_ptr<ChElementLibreSelect> data = std::make_shared<ChElementLibreSelect>();

	PIYING_CLICK_BEHAVIOR(ChElementLibreSelect);
	PIYING_MOVEMOUSE_BEHAVIOR(ChElementLibreSelect);
	PIYING_RELEASE_BEHAVIOR(ChElementLibreSelect);
	PIYING_ESCAPE_BEHAVIOR(ChElementLibreSelect);
	PIYING_DELETE_BEHAVIOR(ChElementLibreSelect);
	PIYING_DRAW_BEHAVIOR(ChElementLibreSelect);

	clickBehavior = std::make_unique<ChElementLibreSelectClick>(data);
	escapeBehavior = std::make_unique<ChElementLibreSelectEscape>(data);
	drawBehavior = std::make_unique<ChElementLibreSelectDraw>(data);
	releaseBehavior = std::make_unique<ChElementLibreSelectRelease>(data);
	moveBehavior = std::make_unique<ChElementLibreSelectMoveMouse>(data);
	deleteBehavior = std::make_unique<ChElementLibreSelectDelete>(data);
}

void ChElementTool::construct_add_vert_trace()
{
	std::shared_ptr<ChAddVertTrace> data = std::make_shared<ChAddVertTrace>();

	PIYING_CLICK_BEHAVIOR(ChAddVertTrace);
	PIYING_MOVEMOUSE_BEHAVIOR(ChAddVertTrace);
	PIYING_RELEASE_BEHAVIOR(ChAddVertTrace);
	PIYING_DRAW_BEHAVIOR(ChAddVertTrace);

	clickBehavior = std::make_unique<ChAddVertTraceClick>(data);
	drawBehavior = std::make_unique<ChAddVertTraceDraw>(data);
	releaseBehavior = std::make_unique<ChAddVertTraceRelease>(data);
	moveBehavior = std::make_unique<ChAddVertTraceMoveMouse>(data);
}

void ChElementTool::construct_rect_select_triangle()
{
	std::shared_ptr<ChTriangleRectSelect> data = std::make_shared<ChTriangleRectSelect>();

	PIYING_CLICK_BEHAVIOR(ChTriangleRectSelect);
	PIYING_MOVEMOUSE_BEHAVIOR(ChTriangleRectSelect);
	PIYING_RELEASE_BEHAVIOR(ChTriangleRectSelect);
	PIYING_ESCAPE_BEHAVIOR(ChTriangleRectSelect);
	PIYING_DELETE_BEHAVIOR(ChTriangleRectSelect);
	PIYING_DRAW_BEHAVIOR(ChTriangleRectSelect);
	PIYING_ENTER_BEHAVIOR(ChTriangleRectSelect);

	clickBehavior = std::make_unique<ChTriangleRectSelectClick>(data);
	escapeBehavior = std::make_unique<ChTriangleRectSelectEscape>(data);
	drawBehavior = std::make_unique<ChTriangleRectSelectDraw>(data);
	releaseBehavior = std::make_unique<ChTriangleRectSelectRelease>(data);
	moveBehavior = std::make_unique<ChTriangleRectSelectMoveMouse>(data);
	deleteBehavior = std::make_unique<ChTriangleRectSelectDelete>(data);
	enterBehavior = std::make_unique<ChTriangleRectSelectEnter>(data);
}

void ChElementTool::construct_libre_select_triangle()
{
	std::shared_ptr<ChTriangleLibreSelect> data = std::make_shared<ChTriangleLibreSelect>();

	PIYING_CLICK_BEHAVIOR(ChTriangleLibreSelect);
	PIYING_MOVEMOUSE_BEHAVIOR(ChTriangleLibreSelect);
	PIYING_RELEASE_BEHAVIOR(ChTriangleLibreSelect);
	PIYING_ENTER_BEHAVIOR(ChTriangleLibreSelect);
	PIYING_DRAW_BEHAVIOR(ChTriangleLibreSelect);
	PIYING_DELETE_BEHAVIOR(ChTriangleLibreSelect);
	PIYING_ESCAPE_BEHAVIOR(ChTriangleLibreSelect);

	clickBehavior = std::make_unique<ChTriangleLibreSelectClick>(data);
	escapeBehavior = std::make_unique<ChTriangleLibreSelectEscape>(data);
	drawBehavior = std::make_unique<ChTriangleLibreSelectDraw>(data);
	releaseBehavior = std::make_unique<ChTriangleLibreSelectRelease>(data);
	moveBehavior = std::make_unique<ChTriangleLibreSelectMoveMouse>(data);
	deleteBehavior = std::make_unique<ChTriangleLibreSelectDelete>(data);
	enterBehavior = std::make_unique<ChTriangleLibreSelectEnter>(data);
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
	if(clickBehavior) clickBehavior->act(mouse);
}

void ChElementTool::release(const QPointF& mouse)
{
	if (releaseBehavior) releaseBehavior->act(mouse);
}

void ChElementTool::move(const QPointF& mouse)
{
	if (moveBehavior) moveBehavior->act(mouse);
}

void ChElementTool::draw()
{
	if(drawBehavior) drawBehavior->act();
}

void ChElementTool::escape()
{
	if(escapeBehavior) escapeBehavior->act();
}

void ChElementTool::delete_element() 
{
	if (deleteBehavior) deleteBehavior->act();
}

void ChElementTool::enter() 
{
	if (enterBehavior) enterBehavior->act();
}
