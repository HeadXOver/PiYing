#include "piying_tool.h"

#include "piYing.h"
#include "piYingGL.h"

#include "tool_add_triangle.h"
#include "tool_add_poly.h"
#include "tool_vert_select.h"
#include "tool_triangle_select.h"
#include "tool_vert_rect_select.h"
#include "tool_vert_libre_select.h"
#include "tool_triangle_rect_select.h"
#include "tool_triangle_libre_select.h"
#include "tool_add_round.h"
#include "tool_add_vert_trace.h"
#include "tool_part_vert_rect_select.h"
#include "tool_part_vert_libre_select.h"
#include "tool_part_triangle_rect_select.h"
#include "tool_part_triangle_libre_select.h"
#include "tool_part_handle.h"

#include "piying_tool_behavior.h"

#include "enum_character_texture_tool_state.h"

#include <qpointf>

void piying::tool::PiyingTool::construct_void()
{
	clickBehavior = std::make_unique<piying::tool::VoidPosBehavior>();
	moveBehavior = std::make_unique<piying::tool::VoidPosBehavior>();
	releaseBehavior = std::make_unique<piying::tool::VoidPosBehavior>();
	escapeBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	deleteBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	enterBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	drawBehavior = std::make_unique<piying::tool::VoidActBehavior>();
}

void piying::tool::PiyingTool::construct_add_triangle()
{
	std::shared_ptr<piying::tool::texture::AddTriangle> data = std::make_shared<piying::tool::texture::AddTriangle>();

	moveBehavior = std::make_unique<piying::tool::VoidPosBehavior>();
	releaseBehavior = std::make_unique<piying::tool::VoidPosBehavior>();
	enterBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	clickBehavior = std::make_unique<piying::tool::Click<piying::tool::texture::AddTriangle>>(data);
	escapeBehavior = std::make_unique<piying::tool::Escape<piying::tool::texture::AddTriangle>>(data);
	deleteBehavior = std::make_unique<piying::tool::Delete<piying::tool::texture::AddTriangle>>(data);
	drawBehavior = std::make_unique<piying::tool::Draw<piying::tool::texture::AddTriangle>>(data);
}

void piying::tool::PiyingTool::construct_add_poly()
{
	std::shared_ptr<piying::tool::texture::AddPoly> data = std::make_shared<piying::tool::texture::AddPoly>();

	moveBehavior = std::make_unique<piying::tool::VoidPosBehavior>();
	releaseBehavior = std::make_unique<piying::tool::VoidPosBehavior>();
	escapeBehavior = std::make_unique<piying::tool::Escape<piying::tool::texture::AddPoly>>(data);
	deleteBehavior = std::make_unique<piying::tool::Delete<piying::tool::texture::AddPoly>>(data);
	clickBehavior = std::make_unique<piying::tool::Click<piying::tool::texture::AddPoly>>(data);
	drawBehavior = std::make_unique<piying::tool::Draw<piying::tool::texture::AddPoly>>(data);
	enterBehavior = std::make_unique<piying::tool::Enter<piying::tool::texture::AddPoly>>(data);
}

void piying::tool::PiyingTool::construct_add_round()
{
	std::shared_ptr<piying::tool::AddRound> data = std::make_shared<piying::tool::AddRound>();

	escapeBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	deleteBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	enterBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	clickBehavior = std::make_unique<piying::tool::Click<piying::tool::AddRound>>(data);
	moveBehavior = std::make_unique<piying::tool::MouseMove<piying::tool::AddRound>>(data);
	releaseBehavior = std::make_unique<piying::tool::Release<piying::tool::AddRound>>(data);
	drawBehavior = std::make_unique<piying::tool::Draw<piying::tool::AddRound>>(data);
}

void piying::tool::PiyingTool::construct_rect_select()
{
	std::shared_ptr<piying::tool::texture::VertRectSelect> data = std::make_shared<piying::tool::texture::VertRectSelect>();

	enterBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	clickBehavior = std::make_unique<piying::tool::Click<piying::tool::texture::VertRectSelect>>(data);
	moveBehavior = std::make_unique<piying::tool::MouseMove<piying::tool::texture::VertRectSelect>>(data);
	releaseBehavior = std::make_unique<piying::tool::Release<piying::tool::texture::VertRectSelect>>(data);
	escapeBehavior = std::make_unique<piying::tool::Escape<piying::tool::texture::VertRectSelect>>(data);
	deleteBehavior = std::make_unique<piying::tool::Delete<piying::tool::texture::VertRectSelect>>(data);
	drawBehavior = std::make_unique<piying::tool::Draw<piying::tool::texture::VertRectSelect>>(data);
}

void piying::tool::PiyingTool::construct_libre_select()
{
	std::shared_ptr<piying::tool::texture::VertLibreSelect> data = std::make_shared<piying::tool::texture::VertLibreSelect>();

	enterBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	clickBehavior = std::make_unique<piying::tool::Click<piying::tool::texture::VertLibreSelect>>(data);
	moveBehavior = std::make_unique<piying::tool::MouseMove<piying::tool::texture::VertLibreSelect>>(data);
	releaseBehavior = std::make_unique<piying::tool::Release<piying::tool::texture::VertLibreSelect>>(data);
	escapeBehavior = std::make_unique<piying::tool::Escape<piying::tool::texture::VertLibreSelect>>(data);
	deleteBehavior = std::make_unique<piying::tool::Delete<piying::tool::texture::VertLibreSelect>>(data);
	drawBehavior = std::make_unique<piying::tool::Draw<piying::tool::texture::VertLibreSelect>>(data);
}

void piying::tool::PiyingTool::construct_add_vert_trace()
{
	std::shared_ptr<piying::tool::part::AddVertTrace> data = std::make_shared<piying::tool::part::AddVertTrace>();

	escapeBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	deleteBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	enterBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	clickBehavior = std::make_unique<piying::tool::Click<piying::tool::part::AddVertTrace>>(data);
	moveBehavior = std::make_unique<piying::tool::MouseMove<piying::tool::part::AddVertTrace>>(data);
	releaseBehavior = std::make_unique<piying::tool::Release<piying::tool::part::AddVertTrace>>(data);
	drawBehavior = std::make_unique<piying::tool::Draw<piying::tool::part::AddVertTrace>>(data);
}

void piying::tool::PiyingTool::construct_rect_select_triangle()
{
	std::shared_ptr<piying::tool::texture::TriangleRectSelect> data = std::make_shared<piying::tool::texture::TriangleRectSelect>();

	clickBehavior = std::make_unique<piying::tool::Click<piying::tool::texture::TriangleRectSelect>>(data);
	moveBehavior = std::make_unique<piying::tool::MouseMove<piying::tool::texture::TriangleRectSelect>>(data);
	escapeBehavior = std::make_unique<piying::tool::Escape<piying::tool::texture::TriangleRectSelect>>(data);
	deleteBehavior = std::make_unique<piying::tool::Delete<piying::tool::texture::TriangleRectSelect>>(data);
	drawBehavior = std::make_unique<piying::tool::Draw<piying::tool::texture::TriangleRectSelect>>(data);
	enterBehavior = std::make_unique<piying::tool::Enter<piying::tool::texture::TriangleRectSelect>>(data);
	releaseBehavior = std::make_unique<piying::tool::Release<piying::tool::texture::TriangleRectSelect>>(data);
}

void piying::tool::PiyingTool::construct_libre_select_triangle()
{
	std::shared_ptr<piying::tool::texture::TriangleLibreSelect> data = std::make_shared<piying::tool::texture::TriangleLibreSelect>();

	clickBehavior = std::make_unique<piying::tool::Click<piying::tool::texture::TriangleLibreSelect>>(data);
	moveBehavior = std::make_unique<piying::tool::MouseMove<piying::tool::texture::TriangleLibreSelect>>(data);
	releaseBehavior = std::make_unique<piying::tool::Release<piying::tool::texture::TriangleLibreSelect>>(data);
	escapeBehavior = std::make_unique<piying::tool::Escape<piying::tool::texture::TriangleLibreSelect>>(data);
	deleteBehavior = std::make_unique<piying::tool::Delete<piying::tool::texture::TriangleLibreSelect>>(data);
	drawBehavior = std::make_unique<piying::tool::Draw<piying::tool::texture::TriangleLibreSelect>>(data);
	enterBehavior = std::make_unique<piying::tool::Enter<piying::tool::texture::TriangleLibreSelect>>(data);
}

void piying::tool::PiyingTool::construct_libre_select_vert_part()
{
	std::shared_ptr<piying::tool::part::VertLibreSelect> data = std::make_shared<piying::tool::part::VertLibreSelect>();

	enterBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	clickBehavior = std::make_unique<piying::tool::Click<piying::tool::part::VertLibreSelect>>(data);
	moveBehavior = std::make_unique<piying::tool::MouseMove<piying::tool::part::VertLibreSelect>>(data);
	releaseBehavior = std::make_unique<piying::tool::Release<piying::tool::part::VertLibreSelect>>(data);
	escapeBehavior = std::make_unique<piying::tool::Escape<piying::tool::part::VertLibreSelect>>(data);
	deleteBehavior = std::make_unique<piying::tool::Delete<piying::tool::part::VertLibreSelect>>(data);
	drawBehavior = std::make_unique<piying::tool::Draw<piying::tool::part::VertLibreSelect>>(data);
}

void piying::tool::PiyingTool::construct_rect_select_vert_part()
{
	std::shared_ptr<piying::tool::part::VertRectSelect> data = std::make_shared<piying::tool::part::VertRectSelect>();

	enterBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	clickBehavior = std::make_unique<piying::tool::Click<piying::tool::part::VertRectSelect>>(data);
	moveBehavior = std::make_unique<piying::tool::MouseMove<piying::tool::part::VertRectSelect>>(data);
	releaseBehavior = std::make_unique<piying::tool::Release<piying::tool::part::VertRectSelect>>(data);
	escapeBehavior = std::make_unique<piying::tool::Escape<piying::tool::part::VertRectSelect>>(data);
	deleteBehavior = std::make_unique<piying::tool::Delete<piying::tool::part::VertRectSelect>>(data);
	drawBehavior = std::make_unique<piying::tool::Draw<piying::tool::part::VertRectSelect>>(data);
}

void piying::tool::PiyingTool::construct_libre_select_triangle_part()
{
	std::shared_ptr<piying::tool::part::TriangleLibreSelect> data = std::make_shared<piying::tool::part::TriangleLibreSelect>();

	enterBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	clickBehavior = std::make_unique<piying::tool::Click<piying::tool::part::TriangleLibreSelect>>(data);
	moveBehavior = std::make_unique<piying::tool::MouseMove<piying::tool::part::TriangleLibreSelect>>(data);
	releaseBehavior = std::make_unique<piying::tool::Release<piying::tool::part::TriangleLibreSelect>>(data);
	escapeBehavior = std::make_unique<piying::tool::Escape<piying::tool::part::TriangleLibreSelect>>(data);
	deleteBehavior = std::make_unique<piying::tool::Delete<piying::tool::part::TriangleLibreSelect>>(data);
	drawBehavior = std::make_unique<piying::tool::Draw<piying::tool::part::TriangleLibreSelect>>(data);
}

void piying::tool::PiyingTool::construct_rect_select_triangle_part()
{
	std::shared_ptr<piying::tool::part::TriangleRectSelect> data = std::make_shared<piying::tool::part::TriangleRectSelect>();

	enterBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	clickBehavior = std::make_unique<piying::tool::Click<piying::tool::part::TriangleRectSelect>>(data);
	moveBehavior = std::make_unique<piying::tool::MouseMove<piying::tool::part::TriangleRectSelect>>(data);
	releaseBehavior = std::make_unique<piying::tool::Release<piying::tool::part::TriangleRectSelect>>(data);
	escapeBehavior = std::make_unique<piying::tool::Escape<piying::tool::part::TriangleRectSelect>>(data);
	deleteBehavior = std::make_unique<piying::tool::Delete<piying::tool::part::TriangleRectSelect>>(data);
	drawBehavior = std::make_unique<piying::tool::Draw<piying::tool::part::TriangleRectSelect>>(data);
}

void piying::tool::PiyingTool::construct_part_handle()
{
	std::shared_ptr<piying::tool::part::Handle> data = std::make_shared<piying::tool::part::Handle>();

	enterBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	escapeBehavior = std::make_unique<piying::tool::VoidActBehavior>();
	clickBehavior = std::make_unique<piying::tool::Click<piying::tool::part::Handle>>(data);
	moveBehavior = std::make_unique<piying::tool::MouseMove<piying::tool::part::Handle>>(data);
	releaseBehavior = std::make_unique<piying::tool::Release<piying::tool::part::Handle>>(data);
	deleteBehavior = std::make_unique<piying::tool::Delete<piying::tool::part::Handle>>(data);
	drawBehavior = std::make_unique<piying::tool::Draw<piying::tool::part::Handle>>(data);
}

piying::tool::PiyingTool::PiyingTool(CharacterToolState chToolState)
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
	case CharacterToolState::RectSelectVertPart: construct_rect_select_vert_part(); return;
	case CharacterToolState::LibreSelectVertPart: construct_libre_select_vert_part(); return;
	case CharacterToolState::RectSelectTrianglePart: construct_rect_select_triangle_part(); return;
	case CharacterToolState::LibreSelectTrianglePart: construct_libre_select_triangle_part(); return;
	case CharacterToolState::PartHandle: construct_part_handle(); return;
	default: construct_void(); return;
	}
}

piying::tool::PiyingTool::~PiyingTool() {}

void piying::tool::PiyingTool::click(const QPointF& mouse) {
	clickBehavior->act(mouse);
}

void piying::tool::PiyingTool::release(const QPointF& mouse)
{
	releaseBehavior->act(mouse);
}

void piying::tool::PiyingTool::move(const QPointF& mouse)
{
	moveBehavior->act(mouse);
}

void piying::tool::PiyingTool::draw()
{
	drawBehavior->act();
}

void piying::tool::PiyingTool::escape()
{
	escapeBehavior->act();
}

void piying::tool::PiyingTool::delete_element()
{
	deleteBehavior->act();
}

void piying::tool::PiyingTool::enter()
{
	enterBehavior->act();
}
