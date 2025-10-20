#include "ch_element_tool.h"
#include "piYing.h"
#include "piYingGL.h"
#include "add_triangle.h"
#include "add_ch_tex_poly.h"
#include "ch_element_rect_select.h"
#include "ch_element_libre_select.h"
#include "ch_element_add_round.h"
#include "gl_vert_reference.h"

#include <memory>
#include <qpointf>

namespace {

	constexpr void(*construct[])(ChElementTool* chElementTool) = {
		nullptr,
		[](ChElementTool* chElementTool) {chElementTool->construct_add_triangle(); },
		[](ChElementTool* chElementTool) {chElementTool->construct_rect_select(); },
		[](ChElementTool* chElementTool) {chElementTool->construct_rect_select(); },
		[](ChElementTool* chElementTool) {chElementTool->construct_libre_select(); },
		[](ChElementTool* chElementTool) {chElementTool->construct_libre_select(); },
		[](ChElementTool* chElementTool) {chElementTool->construct_add_poly(); },
		[](ChElementTool* chElementTool) {chElementTool->construct_add_round(); }
	};

	using stuct_handler = void(*)(ChElementTool*);
	constexpr stuct_handler map_construct(CharacterToolState state) {
		return construct[static_cast<int>(state)];
	}

}

void ChElementTool::construct_add_triangle()
{
	std::shared_ptr<AddTriangle> addTriangle = std::make_shared<AddTriangle>(*glVertReference);
	clickBehavior = new AddTriangleClick(addTriangle);
	escapeBehavior = new AddTriangleEscape(addTriangle);
	drawBehavior = new AddTriangleDraw(addTriangle);
	deleteBehavior = new AddTriangleDelete(addTriangle);
}

void ChElementTool::construct_add_poly()
{
	std::shared_ptr<AddChTexPoly> addPoly = std::make_shared<AddChTexPoly>(*glVertReference);
	clickBehavior = new AddPolyClick(addPoly);
	escapeBehavior = new AddPolyEscape(addPoly);
	drawBehavior = new AddPolyDraw(addPoly);
	deleteBehavior = new AddPolyDelete(addPoly);
	enterBehavior = new AddPolyEnter(addPoly);
}

void ChElementTool::construct_add_round()
{
	std::shared_ptr<ChElementAddRound> addRound = std::make_shared<ChElementAddRound>(*glVertReference);
	clickBehavior = new AddRoundClick(addRound);
	moveBehavior = new AddRoundMove(addRound);
	releaseBehavior = new AddRoundRelease(addRound);
	drawBehavior = new AddRoundDraw(addRound);
}

void ChElementTool::construct_rect_select()
{
	std::shared_ptr<ChElementRectSelect> rectSelect = std::make_shared<ChElementRectSelect>(*glVertReference);
	clickBehavior = new RectSelectClick(rectSelect);
	escapeBehavior = new RectSelectEscape(rectSelect);
	drawBehavior = new RectSelectDraw(rectSelect);
	releaseBehavior = new RectSelectRelease(rectSelect);
	moveBehavior = new RectSelectMove(rectSelect);
	deleteBehavior = new RectSelectDelete(rectSelect);
}

void ChElementTool::construct_libre_select()
{
	std::shared_ptr<ChElementLibreSelect> libreSelect = std::make_shared<ChElementLibreSelect>(*glVertReference);
	clickBehavior = new LibreSelectClick(libreSelect);
	escapeBehavior = new LibreSelectEscape(libreSelect);
	drawBehavior = new LibreSelectDraw(libreSelect);
	releaseBehavior = new LibreSelectRelease(libreSelect);
	moveBehavior = new LibreSelectMove(libreSelect);
	deleteBehavior = new LibreSelectDelete(libreSelect);
}

ChElementTool::ChElementTool(int current, PiYingGL& pygl, CharacterToolState chToolState) : glVertReference(new GlVertReference(current, pygl))
{
	if (chToolState == CharacterToolState::None) return;

	map_construct(chToolState)(this);
}

ChElementTool::~ChElementTool()
{
	if (clickBehavior) delete clickBehavior;
	if (moveBehavior) delete moveBehavior;
	if (releaseBehavior) delete releaseBehavior;
	if (escapeBehavior) delete escapeBehavior;
	if (deleteBehavior) delete deleteBehavior;
	if (enterBehavior) delete enterBehavior;
	if (drawBehavior) delete drawBehavior;
	if (glVertReference) delete glVertReference;
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

void ChElementTool::draw(QPainter* painter)
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
