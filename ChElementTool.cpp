#include "ChElementTool.h"
#include "piYing.h"
#include "piYingGL.h"
#include "AddTriangle.h"
#include "AddChTexPoly.h"
#include "ChElementRectSelect.h"
#include "ChElementLibreSelect.h"
#include "ChElementAddRound.h"
#include "glVertReference.h"

#include <memory>
#include <qpointf>

ChElementTool::ChElementTool(int current, PiYingGL& pygl, CharacterToolState chToolState) : glVertReference(new GlVertReference(current, pygl))
{
	if (chToolState == CharacterToolState::AddTriangle) {
		std::shared_ptr<AddTriangle> addTriangle = std::make_shared<AddTriangle>(*glVertReference);
		clickBehavior = new AddTriangleClick(addTriangle);
		escapeBehavior = new AddTriangleEscape(addTriangle);
		drawBehavior = new AddTriangleDraw(addTriangle);
		deleteBehavior = new AddTriangleDelete(addTriangle);
	}
	else if (chToolState == CharacterToolState::AddPoly) {
		std::shared_ptr<AddChTexPoly> addPoly = std::make_shared<AddChTexPoly>(*glVertReference);
		clickBehavior = new AddPolyClick(addPoly);
		escapeBehavior = new AddPolyEscape(addPoly);
		drawBehavior = new AddPolyDraw(addPoly);
		deleteBehavior = new AddPolyDelete(addPoly);
		enterBehavior = new AddPolyEnter(addPoly);
	}
	else if (chToolState == CharacterToolState::RectSelectVert) {
		std::shared_ptr<ChElementRectSelect> rectSelect = std::make_shared<ChElementRectSelect>(*glVertReference);
		clickBehavior = new RectSelectClick(rectSelect);
		escapeBehavior = new RectSelectEscape(rectSelect);
		drawBehavior = new RectSelectDraw(rectSelect);
		releaseBehavior = new RectSelectRelease(rectSelect);
		moveBehavior = new RectSelectMove(rectSelect);
		deleteBehavior = new RectSelectDelete(rectSelect);
	}
	else if (chToolState == CharacterToolState::LibreSelectVert) {
		std::shared_ptr<ChElementLibreSelect> libreSelect = std::make_shared<ChElementLibreSelect>(*glVertReference);
		clickBehavior = new LibreSelectClick(libreSelect);
		escapeBehavior = new LibreSelectEscape(libreSelect);
		drawBehavior = new LibreSelectDraw(libreSelect);
		releaseBehavior = new LibreSelectRelease(libreSelect);
		moveBehavior = new LibreSelectMove(libreSelect);
		deleteBehavior = new LibreSelectDelete(libreSelect);
	}
	else if (chToolState == CharacterToolState::AddRound) {
		std::shared_ptr<ChElementAddRound> addRound = std::make_shared<ChElementAddRound>(*glVertReference);
		clickBehavior = new AddRoundClick(addRound);
		moveBehavior = new AddRoundMove(addRound);
		releaseBehavior = new AddRoundRelease(addRound);
		drawBehavior = new AddRoundDraw(addRound);
	}
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

void ChElementTool::deleteElement() 
{
	if (deleteBehavior) deleteBehavior->deleteElement();
}

void ChElementTool::enter() 
{
	if (enterBehavior) enterBehavior->enter();
}
