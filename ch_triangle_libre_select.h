#pragma once

#include <memory>
#include <qpolygonf>

#include "ch_element_tool_behavior.h"

struct ChTriangleSelect;

struct ChTriangleLibreSelect final
{
public:
	ChTriangleLibreSelect();
	~ChTriangleLibreSelect() = default;

	void draw();
	void clickPos(const QPointF& mouse);
	void movePos(const QPointF& mouse);
	void releasePos(const QPointF& mouse);
	void enter();

	std::unique_ptr<ChTriangleSelect> chTriangleSelect;
	QPolygonF polygon;
	bool drawing = false;
	bool edit_skelen{ false };
	bool isPress = false;
};

///////////////////////////////////////////////////

class LibreSelectTriangleClick final : public ClickBehavior
{
public:
	LibreSelectTriangleClick(const std::shared_ptr<ChTriangleLibreSelect> select) : libreSelect(select) {}

	virtual void click(const QPointF& point) override;

private:
	const std::shared_ptr<ChTriangleLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectTriangleMove final : public MouseMoveBehavior
{
public:
	LibreSelectTriangleMove(const std::shared_ptr<ChTriangleLibreSelect> select) : libreSelect(select) {}

	virtual void mouse_move(const QPointF& point) override;

private:
	const std::shared_ptr<ChTriangleLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectTriangleRelease final : public ReleaseBehavior
{
public:
	LibreSelectTriangleRelease(const std::shared_ptr<ChTriangleLibreSelect> select) : libreSelect(select) {}

	virtual void release(const QPointF& point) override;

private:
	const std::shared_ptr<ChTriangleLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectTriangleDelete final : public DeleteElementBehavior
{
public:
	LibreSelectTriangleDelete(const std::shared_ptr<ChTriangleLibreSelect> select) : libreSelect(select) {}


	virtual void delete_element() override;

private:
	const std::shared_ptr<ChTriangleLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectTriangleEscape final : public EscapeBehavior
{
public:
	LibreSelectTriangleEscape(const std::shared_ptr<ChTriangleLibreSelect> select) : libreSelect(select) {}


	virtual void escape() override;

private:
	const std::shared_ptr<ChTriangleLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectTriangleDraw final : public DrawBehavior
{
public:
	LibreSelectTriangleDraw(const std::shared_ptr<ChTriangleLibreSelect> select) : libreSelect(select) {}
	virtual void draw() override;

private:
	const std::shared_ptr<ChTriangleLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectTriangleEnter final : public EnterBehavior
{
public:
	LibreSelectTriangleEnter(const std::shared_ptr<ChTriangleLibreSelect> select) : libreSelect(select) {}
	virtual void enter() override;

private:
	const std::shared_ptr<ChTriangleLibreSelect> libreSelect;
};

