#pragma once

#include <memory>
#include <qpointf>

#include "ch_element_tool_behavior.h"

struct ChTriangleSelect;

struct ChTriangleRectSelect final
{
	ChTriangleRectSelect();
	~ChTriangleRectSelect() = default;

	void draw();
	void clickPos(const QPointF& mouse);
	void movePos(const QPointF& mouse);
	void releasePos(const QPointF& mouse);
	void enter();

	std::unique_ptr<ChTriangleSelect> chTriangleSelect;
	const bool edit_skelen{ false };
	QPointF rect;
	bool isDraw = false;
};

///////////////////////////////////////////////////

class RectSelectTriangleClick final : public ClickBehavior
{
public:
	RectSelectTriangleClick(const std::shared_ptr<ChTriangleRectSelect> rect_select) : rectSelect(rect_select) {}

	virtual void click(const QPointF& point) override;

private:
	const std::shared_ptr<ChTriangleRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectTriangleMove final : public MouseMoveBehavior
{
public:
	RectSelectTriangleMove(const std::shared_ptr<ChTriangleRectSelect> rect_select) : rectSelect(rect_select) {}

	virtual void mouseMove(const QPointF& point) override;

private:
	const std::shared_ptr<ChTriangleRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectTriangleRelease final : public ReleaseBehavior
{
public:
	RectSelectTriangleRelease(const std::shared_ptr<ChTriangleRectSelect> rect_select) : rectSelect(rect_select) {}

	virtual void release(const QPointF& point) override;

private:
	const std::shared_ptr<ChTriangleRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectTriangleDelete final : public DeleteElementBehavior
{
public:
	RectSelectTriangleDelete(const std::shared_ptr<ChTriangleRectSelect> rect_select) : rectSelect(rect_select) {}


	virtual void delete_element() override;

private:
	const std::shared_ptr<ChTriangleRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectTriangleEscape final : public EscapeBehavior
{
public:
	RectSelectTriangleEscape(const std::shared_ptr<ChTriangleRectSelect> rect_select) : rectSelect(rect_select) {}
	virtual void escape() override;

private:
	const std::shared_ptr<ChTriangleRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectTriangleDraw final : public DrawBehavior
{
public:
	RectSelectTriangleDraw(const std::shared_ptr<ChTriangleRectSelect> rect_select) : rectSelect(rect_select) {}
	virtual void draw() override;

private:
	const std::shared_ptr<ChTriangleRectSelect> rectSelect;
};

///////////////////////////////////////////////////

class RectSelectTriangleEnter final : public EnterBehavior
{
public:
	RectSelectTriangleEnter(const std::shared_ptr<ChTriangleRectSelect> rect_select) : rectSelect(rect_select) {}
	virtual void enter() override;

private:
	const std::shared_ptr<ChTriangleRectSelect> rectSelect;
};


