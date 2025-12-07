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
#include "ch_element_add_round.h"
#include "ch_add_vert_trace.h"
#include "gl_vert_reference.h"
#include "SelectedPoints.h"

#include <memory>
#include <qpointf>

namespace {

	constexpr void(*construct[])(ChElementTool* chElementTool) = {
		nullptr,
		[](ChElementTool* chElementTool) {chElementTool->construct_add_triangle(); },
		[](ChElementTool* chElementTool) {chElementTool->construct_rect_select(); },
		[](ChElementTool* chElementTool) {chElementTool->construct_libre_select(); },
		[](ChElementTool* chElementTool) {chElementTool->construct_add_poly(); },
		[](ChElementTool* chElementTool) {chElementTool->construct_add_round(); },
		[](ChElementTool* chElementTool) {chElementTool->construct_add_vert_trace(); },
		[](ChElementTool* chElementTool) {chElementTool->construct_rect_select_triangle(); },
		[](ChElementTool* chElementTool) {chElementTool->construct_libre_select_triangle(); }
	};

	using stuct_handler = void(*)(ChElementTool*);
	constexpr stuct_handler map_construct(CharacterToolState state) {
		return construct[static_cast<int>(state)];
	}

}

void ChElementTool::construct_add_triangle()
{
	std::shared_ptr<AddTriangle> addTriangle = std::make_shared<AddTriangle>(*glVertReference);
	clickBehavior = std::make_unique<AddTriangleClick>(addTriangle);
	escapeBehavior = std::make_unique<AddTriangleEscape>(addTriangle);
	drawBehavior = std::make_unique<AddTriangleDraw>(addTriangle);
	deleteBehavior = std::make_unique<AddTriangleDelete>(addTriangle);
}

void ChElementTool::construct_add_poly()
{
	std::shared_ptr<AddChTexPoly> addPoly = std::make_shared<AddChTexPoly>(*glVertReference);
	clickBehavior = std::make_unique<AddPolyClick>(addPoly);
	escapeBehavior = std::make_unique<AddPolyEscape>(addPoly);
	drawBehavior = std::make_unique<AddPolyDraw>(addPoly);
	deleteBehavior = std::make_unique<AddPolyDelete>(addPoly);
	enterBehavior = std::make_unique<AddPolyEnter>(addPoly);
}

void ChElementTool::construct_add_round()
{
	std::shared_ptr<ChElementAddRound> addRound = std::make_shared<ChElementAddRound>(*glVertReference);
	clickBehavior = std::make_unique<AddRoundClick>(addRound);
	moveBehavior = std::make_unique<AddRoundMove>(addRound);
	releaseBehavior = std::make_unique<AddRoundRelease>(addRound);
	drawBehavior = std::make_unique<AddRoundDraw>(addRound);
}

void ChElementTool::construct_rect_select()
{
	std::shared_ptr<ChElementRectSelect> rectSelect = std::make_shared<ChElementRectSelect>(*glVertReference);
	clickBehavior = std::make_unique<RectSelectClick>(rectSelect);
	escapeBehavior = std::make_unique<RectSelectEscape>(rectSelect);
	drawBehavior = std::make_unique<RectSelectDraw>(rectSelect);
	releaseBehavior = std::make_unique<RectSelectRelease>(rectSelect);
	moveBehavior = std::make_unique<RectSelectMove>(rectSelect);
	deleteBehavior = std::make_unique<RectSelectDelete>(rectSelect);
	enterBehavior = std::make_unique<RectSelectEnter>(rectSelect);
}

void ChElementTool::construct_libre_select()
{
	std::shared_ptr<ChElementLibreSelect> libreSelect = std::make_shared<ChElementLibreSelect>(*glVertReference);
	clickBehavior = std::make_unique<LibreSelectClick>(libreSelect);
	escapeBehavior = std::make_unique<LibreSelectEscape>(libreSelect);
	drawBehavior = std::make_unique<LibreSelectDraw>(libreSelect);
	releaseBehavior = std::make_unique<LibreSelectRelease>(libreSelect);
	moveBehavior = std::make_unique<LibreSelectMove>(libreSelect);
	deleteBehavior = std::make_unique<LibreSelectDelete>(libreSelect);
	enterBehavior = std::make_unique<LibreSelectEnter>(libreSelect);
}

void ChElementTool::construct_add_vert_trace()
{
	std::shared_ptr<ChAddVertTrace> addVertTrace = std::make_shared<ChAddVertTrace>(*glVertReference);
	clickBehavior = std::make_unique<AddVertTraceClick>(addVertTrace);
	drawBehavior = std::make_unique<AddVertTraceDraw>(addVertTrace);
	releaseBehavior = std::make_unique<AddVertTraceRelease>(addVertTrace);
	moveBehavior = std::make_unique<AddVertTraceMove>(addVertTrace);
}

void ChElementTool::construct_rect_select_triangle()
{
	std::shared_ptr<ChTriangleRectSelect> rectSelect = std::make_shared<ChTriangleRectSelect>(*glVertReference);
	clickBehavior = std::make_unique<RectSelectTriangleClick>(rectSelect);
	escapeBehavior = std::make_unique<RectSelectTriangleEscape>(rectSelect);
	drawBehavior = std::make_unique<RectSelectTriangleDraw>(rectSelect);
	releaseBehavior = std::make_unique<RectSelectTriangleRelease>(rectSelect);
	moveBehavior = std::make_unique<RectSelectTriangleMove>(rectSelect);
	deleteBehavior = std::make_unique<RectSelectTriangleDelete>(rectSelect);
	enterBehavior = std::make_unique<RectSelectTriangleEnter>(rectSelect);
}

void ChElementTool::construct_libre_select_triangle()
{
	std::shared_ptr<ChElementLibreSelect> libreSelect = std::make_shared<ChElementLibreSelect>(*glVertReference);
	clickBehavior = std::make_unique<LibreSelectClick>(libreSelect);
	escapeBehavior = std::make_unique<LibreSelectEscape>(libreSelect);
	drawBehavior = std::make_unique<LibreSelectDraw>(libreSelect);
	releaseBehavior = std::make_unique<LibreSelectRelease>(libreSelect);
	moveBehavior = std::make_unique<LibreSelectMove>(libreSelect);
	deleteBehavior = std::make_unique<LibreSelectDelete>(libreSelect);
	enterBehavior = std::make_unique<LibreSelectEnter>(libreSelect);
}

ChElementTool::ChElementTool(int current, PiYingGL& pygl, CharacterToolState chToolState)
{
	glVertReference = std::make_unique<GlVertReference>(current, pygl);

	if (chToolState == CharacterToolState::None) return;

	map_construct(chToolState)(this);
}

ChElementTool::~ChElementTool()
{
}

void ChElementTool::click(const QPointF& mouse) {
	if(clickBehavior) clickBehavior->click(mouse);
}

void ChElementTool::release(const QPointF& mouse)
{
	if (releaseBehavior) releaseBehavior->release(mouse);
}

void ChElementTool::move(const QPointF& mouse)
{
	if (moveBehavior) moveBehavior->mouseMove(mouse);
}

void ChElementTool::draw(QPainter& painter)
{
	if(drawBehavior) drawBehavior->draw(painter);
}

void ChElementTool::escape()
{
	if(escapeBehavior) escapeBehavior->escape();
}

void ChElementTool::delete_element() 
{
	if (deleteBehavior) deleteBehavior->deleteElement();
}

void ChElementTool::enter() 
{
	if (enterBehavior) enterBehavior->enter();
}
