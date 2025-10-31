#pragma once

#include <memory>
#include <qpointf>

#include "ch_element_tool_behavior.h"

struct ChElementSelect;
struct GlVertReference;
class QPainter;
class PiYingGL;

struct ChElementRectSelect final
{
	ChElementRectSelect(GlVertReference& glReference);
	~ChElementRectSelect() = default;

	void draw(QPainter& painter);
	void clickPos(const QPointF& mouse);
	void movePos(const QPointF& mouse);
	void releasePos(const QPointF& mouse);

	std::unique_ptr<ChElementSelect> chElementSelect;
	const bool edit_skelen{ false };
	QPointF rect;
	bool isDraw = false;
};

///////////////////////////////////////////////////

class RectSelectClick final : public ClickBehavior
{
public:
	RectSelectClick(std::shared_ptr<ChElementRectSelect> rect_select) { rectSelect = rect_select; }

	virtual void click(const QPointF& point) override;

private:
	std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectMove final : public MouseMoveBehavior
{
public:
	RectSelectMove(std::shared_ptr<ChElementRectSelect> rect_select) { rectSelect = rect_select; }

	virtual void mouseMove(const QPointF& point) override;

private:
	std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectRelease final : public ReleaseBehavior
{
public:
	RectSelectRelease(std::shared_ptr<ChElementRectSelect> rect_select) { rectSelect = rect_select; }

	virtual void release(const QPointF& point) override;

private:
	std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectDelete final : public DeleteElementBehavior
{
public:
	RectSelectDelete(std::shared_ptr<ChElementRectSelect> rect_select) { rectSelect = rect_select; }


	virtual void deleteElement() override;

private:
	std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectEscape final : public EscapeBehavior
{
public:
	RectSelectEscape(std::shared_ptr<ChElementRectSelect> rect_select) { rectSelect = rect_select; }
	virtual void escape() override;

private:
	std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectDraw final : public DrawBehavior
{
public:
	RectSelectDraw(std::shared_ptr<ChElementRectSelect> rect_select) { rectSelect = rect_select; }
	virtual void draw(QPainter& painter) override;

private:
	std::shared_ptr<ChElementRectSelect> rectSelect;
};

 
