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

void ChElementTool::construct_void()
{
	clickBehavior = std::make_unique<PiYingVoidClick>();
	moveBehavior = std::make_unique<PiYingVoidMove>();
	releaseBehavior = std::make_unique<PiYingVoidRelease>();
	escapeBehavior = std::make_unique<PiYingVoidEscape>();
	deleteBehavior = std::make_unique<PiYingVoidDelete>();
	enterBehavior = std::make_unique<PiYingVoidEnter>();
	drawBehavior = std::make_unique<PiYingVoidDraw>();
}

void ChElementTool::construct_add_triangle()
{
	std::shared_ptr<AddTriangle> data = std::make_shared<AddTriangle>();

	moveBehavior = std::make_unique<PiYingVoidMove>();
	releaseBehavior = std::make_unique<PiYingVoidRelease>();
	enterBehavior = std::make_unique<PiYingVoidEnter>();
	clickBehavior = std::make_unique<PiYingToolClick<AddTriangle>>(data);
	escapeBehavior = std::make_unique<PiYingToolEscape<AddTriangle>>(data);
	deleteBehavior = std::make_unique<PiYingToolDelete<AddTriangle>>(data);
	drawBehavior = std::make_unique<PiYingToolDraw<AddTriangle>>(data);
}

void ChElementTool::construct_add_poly()
{
	std::shared_ptr<AddChTexPoly> data = std::make_shared<AddChTexPoly>(); 

	moveBehavior = std::make_unique<PiYingVoidMove>(); 
	releaseBehavior = std::make_unique<PiYingVoidRelease>();
	escapeBehavior = std::make_unique<PiYingToolEscape<AddChTexPoly>>(data);
	deleteBehavior = std::make_unique<PiYingToolDelete<AddChTexPoly>>(data);
	clickBehavior = std::make_unique<PiYingToolClick<AddChTexPoly>>(data);
	drawBehavior = std::make_unique<PiYingToolDraw<AddChTexPoly>>(data);
	enterBehavior = std::make_unique<PiYingToolEnter<AddChTexPoly>>(data);
}

void ChElementTool::construct_add_round()
{
	std::shared_ptr<ChElementAddRound> data = std::make_shared<ChElementAddRound>(); 

	escapeBehavior = std::make_unique<PiYingVoidEscape>(); 
	deleteBehavior = std::make_unique<PiYingVoidDelete>(); 
	enterBehavior = std::make_unique<PiYingVoidEnter>();
	clickBehavior = std::make_unique<PiYingToolClick<ChElementAddRound>>(data);
	moveBehavior = std::make_unique<PiYingToolMouseMove<ChElementAddRound>>(data);
	releaseBehavior = std::make_unique<PiYingToolRelease<ChElementAddRound>>(data);
	drawBehavior = std::make_unique<PiYingToolDraw<ChElementAddRound>>(data);
}

void ChElementTool::construct_rect_select()
{
	std::shared_ptr<ChElementRectSelect> data = std::make_shared<ChElementRectSelect>();

	enterBehavior = std::make_unique<PiYingVoidEnter>();
	clickBehavior = std::make_unique<PiYingToolClick<ChElementRectSelect>>(data);
	moveBehavior = std::make_unique<PiYingToolMouseMove<ChElementRectSelect>>(data);
	releaseBehavior = std::make_unique<PiYingToolRelease<ChElementRectSelect>>(data);
	escapeBehavior = std::make_unique<PiYingToolEscape<ChElementRectSelect>>(data);
	deleteBehavior = std::make_unique<PiYingToolDelete<ChElementRectSelect>>(data);
	drawBehavior = std::make_unique<PiYingToolDraw<ChElementRectSelect>>(data);
}

void ChElementTool::construct_libre_select()
{
	std::shared_ptr<ChElementLibreSelect> data = std::make_shared<ChElementLibreSelect>();

	enterBehavior = std::make_unique<PiYingVoidEnter>();
	clickBehavior = std::make_unique<PiYingToolClick<ChElementLibreSelect>>(data);
	moveBehavior = std::make_unique<PiYingToolMouseMove<ChElementLibreSelect>>(data);
	releaseBehavior = std::make_unique<PiYingToolRelease<ChElementLibreSelect>>(data);
	escapeBehavior = std::make_unique<PiYingToolEscape<ChElementLibreSelect>>(data);
	deleteBehavior = std::make_unique<PiYingToolDelete<ChElementLibreSelect>>(data);
	drawBehavior = std::make_unique<PiYingToolDraw<ChElementLibreSelect>>(data);
}

void ChElementTool::construct_add_vert_trace()
{
	std::shared_ptr<ChAddVertTrace> data = std::make_shared<ChAddVertTrace>();

	escapeBehavior = std::make_unique<PiYingVoidEscape>();
	deleteBehavior = std::make_unique<PiYingVoidDelete>();
	enterBehavior = std::make_unique<PiYingVoidEnter>();
	clickBehavior = std::make_unique<PiYingToolClick<ChAddVertTrace>>(data);
	moveBehavior = std::make_unique<PiYingToolMouseMove<ChAddVertTrace>>(data);
	releaseBehavior = std::make_unique<PiYingToolRelease<ChAddVertTrace>>(data);
	drawBehavior = std::make_unique<PiYingToolDraw<ChAddVertTrace>>(data);
}

void ChElementTool::construct_rect_select_triangle()
{
	std::shared_ptr<ChTriangleRectSelect> data = std::make_shared<ChTriangleRectSelect>();

	clickBehavior = std::make_unique<PiYingToolClick<ChTriangleRectSelect>>(data);
	moveBehavior = std::make_unique<PiYingToolMouseMove<ChTriangleRectSelect>>(data);
	escapeBehavior = std::make_unique<PiYingToolEscape<ChTriangleRectSelect>>(data);
	deleteBehavior = std::make_unique<PiYingToolDelete<ChTriangleRectSelect>>(data);
	drawBehavior = std::make_unique<PiYingToolDraw<ChTriangleRectSelect>>(data);
	enterBehavior = std::make_unique<PiYingToolEnter<ChTriangleRectSelect>>(data);
	releaseBehavior = std::make_unique<PiYingToolRelease<ChTriangleRectSelect>>(data);
}

void ChElementTool::construct_libre_select_triangle()
{
	std::shared_ptr<ChTriangleLibreSelect> data = std::make_shared<ChTriangleLibreSelect>();

	clickBehavior = std::make_unique<PiYingToolClick<ChTriangleLibreSelect>>(data);
	moveBehavior = std::make_unique<PiYingToolMouseMove<ChTriangleLibreSelect>>(data);
	releaseBehavior = std::make_unique<PiYingToolRelease<ChTriangleLibreSelect>>(data);
	escapeBehavior = std::make_unique<PiYingToolEscape<ChTriangleLibreSelect>>(data);
	deleteBehavior = std::make_unique<PiYingToolDelete<ChTriangleLibreSelect>>(data);
	drawBehavior = std::make_unique<PiYingToolDraw<ChTriangleLibreSelect>>(data);
	enterBehavior = std::make_unique<PiYingToolEnter<ChTriangleLibreSelect>>(data);
}

ChElementTool::ChElementTool(CharacterToolState chToolState)
{
	switch (chToolState) {
	case CharacterToolState::None: construct_void(); return;
	case CharacterToolState::AddTriangle: construct_add_triangle(); return;
	case CharacterToolState::AddPoly: construct_add_poly(); return;
	case CharacterToolState::AddRound: construct_add_round(); return;
	case CharacterToolState::RectSelectVert: construct_rect_select(); return;
	case CharacterToolState::LibreSelectVert: construct_libre_select(); return;
	case CharacterToolState::AddVertTrace: construct_add_vert_trace(); return;
	case CharacterToolState::RectSelectTriangle: construct_rect_select_triangle(); return;
	case CharacterToolState::LibreSelectTriangle: construct_libre_select_triangle(); return;
	default: construct_void(); return;
	}
}

ChElementTool::~ChElementTool() {}

void ChElementTool::click(const QPointF& mouse) {
	clickBehavior->act(mouse);
}

void ChElementTool::release(const QPointF& mouse)
{
	releaseBehavior->act(mouse);
}

void ChElementTool::move(const QPointF& mouse)
{
	moveBehavior->act(mouse);
}

void ChElementTool::draw()
{
	drawBehavior->act();
}

void ChElementTool::escape()
{
	escapeBehavior->act();
}

void ChElementTool::delete_element() 
{
	deleteBehavior->act();
}

void ChElementTool::enter() 
{
	enterBehavior->act();
}
