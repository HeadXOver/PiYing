#pragma once

#include <memory>

#include "ChElementToolBehavior.h"

struct ChElementSelect;
struct GlVertReference;
class QPainter;
class QPointF;
class PiYingGL;

struct ChElementRectSelect final
{
	ChElementRectSelect(GlVertReference& glReference);
	~ChElementRectSelect();

	void draw(QPainter* painter);
	void clickPos(const QPointF& mouse);
	void movePos(const QPointF& mouse);
	void releasePos(const QPointF& mouse);

	ChElementSelect* chElementSelect;
	QPointF* rect;
	bool isDraw = false;
};

///////////////////////////////////////////////////

class RectSelectClick : public ClickBehavior
{
public:
	RectSelectClick(std::shared_ptr<ChElementRectSelect> rect_select) { rectSelect = rect_select; }

	virtual void click(const QPointF& point) override;

private:
	std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectMove : public MouseMoveBehavior
{
public:
	RectSelectMove(std::shared_ptr<ChElementRectSelect> rect_select) { rectSelect = rect_select; }

	virtual void mouseMove(const QPointF& point) override;

private:
	std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectRelease : public ReleaseBehavior
{
public:
	RectSelectRelease(std::shared_ptr<ChElementRectSelect> rect_select) { rectSelect = rect_select; }

	virtual void release(const QPointF& point) override;

private:
	std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectDelete : public DeleteElementBehavior
{
public:
	RectSelectDelete(std::shared_ptr<ChElementRectSelect> rect_select) { rectSelect = rect_select; }


	virtual void deleteElement() override;

private:
	std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectEscape : public EscapeBehavior
{
public:
	RectSelectEscape(std::shared_ptr<ChElementRectSelect> rect_select) { rectSelect = rect_select; }


	virtual void escape() override;

private:
	std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectDraw : public DrawBehavior
{
public:
	RectSelectDraw(std::shared_ptr<ChElementRectSelect> rect_select) { rectSelect = rect_select; }
	virtual void draw(QPainter* painter) override;

private:
	std::shared_ptr<ChElementRectSelect> rectSelect;
};

 
