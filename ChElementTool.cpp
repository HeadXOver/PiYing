#include "ChElementTool.h"
#include "piYing.h"
#include "piYingGL.h"
#include "AddTriangle.h"
#include "AddChTexPoly.h"
#include "ChElementRectSelect.h"
#include "ChElementLibreSelect.h"
#include "glVertReference.h"

#include <memory>

ChElementTool::ChElementTool(int current, PiYingGL* pygl, CharacterTextureToolState chToolState) : glVertReference(new GlVertReference(current, pygl))
{
	if (chToolState == CharacterTextureToolState::AddTriangle) {
		std::shared_ptr<AddTriangle> addTriangle = std::make_shared<AddTriangle>(glVertReference);
		clickBehavior = new AddTriangleClick(addTriangle);
		escapeBehavior = new AddTriangleEscape(addTriangle);
		drawBehavior = new AddTriangleDraw(addTriangle);
		deleteBehavior = new AddTriangleDelete(addTriangle);
	}
	else if (chToolState == CharacterTextureToolState::AddPoly) {
		std::shared_ptr<AddChTexPoly> addPoly = std::make_shared<AddChTexPoly>(glVertReference);
		clickBehavior = new AddPolyClick(addPoly);
		escapeBehavior = new AddPolyEscape(addPoly);
		drawBehavior = new AddPolyDraw(addPoly);
		deleteBehavior = new AddPolyDelete(addPoly);
		enterBehavior = new AddPolyEnter(addPoly);
	}
	else if (chToolState == CharacterTextureToolState::RectSelectVert) {
		std::shared_ptr<ChElementRectSelect> rectSelect = std::make_shared<ChElementRectSelect>(glVertReference);
		clickBehavior = new RectSelectClick(rectSelect);
		escapeBehavior = new RectSelectEscape(rectSelect);
		drawBehavior = new RectSelectDraw(rectSelect);
		releaseBehavior = new RectSelectRelease(rectSelect);
		moveBehavior = new RectSelectMove(rectSelect);
		deleteBehavior = new RectSelectDelete(rectSelect);
	}
	else if (chToolState == CharacterTextureToolState::LibreSelectVert) {
		std::shared_ptr<ChElementLibreSelect> libreSelect = std::make_shared<ChElementLibreSelect>(glVertReference);
		clickBehavior = new LibreSelectClick(libreSelect);
		escapeBehavior = new LibreSelectEscape(libreSelect);
		drawBehavior = new LibreSelectDraw(libreSelect);
		releaseBehavior = new LibreSelectRelease(libreSelect);
		moveBehavior = new LibreSelectMove(libreSelect);
		deleteBehavior = new LibreSelectDelete(libreSelect);
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
