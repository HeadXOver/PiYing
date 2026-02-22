#pragma once

#include <memory>
#include <qpointf>

#include "ch_element_tool_behavior.h"

struct ChElementSelect;

struct ChElementRectSelect final
{
	ChElementRectSelect();
	~ChElementRectSelect() = default;

	void draw();
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
	RectSelectClick(const std::shared_ptr<ChElementRectSelect> rect_select) : rectSelect(rect_select) {}

	virtual void click(const QPointF& point) override;

private:
	const std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectMove final : public MouseMoveBehavior
{
public:
	RectSelectMove(const std::shared_ptr<ChElementRectSelect> rect_select) : rectSelect(rect_select) {}

	virtual void mouseMove(const QPointF& point) override;

private:
	const std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectRelease final : public ReleaseBehavior
{
public:
	RectSelectRelease(const std::shared_ptr<ChElementRectSelect> rect_select) : rectSelect(rect_select) {}

	virtual void release(const QPointF& point) override;

private:
	const std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectDelete final : public DeleteElementBehavior
{
public:
	RectSelectDelete(const std::shared_ptr<ChElementRectSelect> rect_select) : rectSelect(rect_select) {}


	virtual void delete_element() override;

private:
	const std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectEscape final : public EscapeBehavior
{
public:
	RectSelectEscape(const std::shared_ptr<ChElementRectSelect> rect_select) : rectSelect(rect_select) {}
	virtual void escape() override;

private:
	const std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectDraw final : public DrawBehavior
{
public:
	RectSelectDraw(const std::shared_ptr<ChElementRectSelect> rect_select) : rectSelect(rect_select) {}
	virtual void draw() override;

private:
	const std::shared_ptr<ChElementRectSelect> rectSelect;
};

///////////////////////////////////////////////////

 
