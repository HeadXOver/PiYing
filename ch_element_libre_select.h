#pragma once

#include <memory>
#include <qpolygonf>

#include "ch_element_tool_behavior.h"

struct ChElementSelect;

struct ChElementLibreSelect final
{
public:
	ChElementLibreSelect();
	~ChElementLibreSelect() = default;

	void draw();
	void clickPos(const QPointF& mouse);
	void movePos(const QPointF& mouse);
	void releasePos(const QPointF& mouse);
	void enter();

	std::unique_ptr<ChElementSelect> chElementSelect;
	QPolygonF polygon;
	bool drawing = false;
	bool edit_skelen{ false }; 
	bool isPress = false;
};

///////////////////////////////////////////////////

class LibreSelectClick final : public ClickBehavior
{
public:
	LibreSelectClick(const std::shared_ptr<ChElementLibreSelect> select) : libreSelect(select) {}

	virtual void click(const QPointF& point) override;

private:
	const std::shared_ptr<ChElementLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectMove final : public MouseMoveBehavior
{
public:
	LibreSelectMove(const std::shared_ptr<ChElementLibreSelect> select) : libreSelect(select) {}

	virtual void mouseMove(const QPointF& point) override;

private:
	const std::shared_ptr<ChElementLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectRelease final : public ReleaseBehavior
{
public:
	LibreSelectRelease(const std::shared_ptr<ChElementLibreSelect> select) : libreSelect(select) {}

	virtual void release(const QPointF& point) override;

private:
	const std::shared_ptr<ChElementLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectDelete final : public DeleteElementBehavior
{
public:
	LibreSelectDelete(const std::shared_ptr<ChElementLibreSelect> select) : libreSelect(select) {}

	virtual void deleteElement() override;

private:
	const std::shared_ptr<ChElementLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectEscape final : public EscapeBehavior
{
public:
	LibreSelectEscape(const std::shared_ptr<ChElementLibreSelect> select) : libreSelect(select) {}

	virtual void escape() override;

private:
	const std::shared_ptr<ChElementLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectDraw final : public DrawBehavior
{
public:
	LibreSelectDraw(const std::shared_ptr<ChElementLibreSelect> select) : libreSelect(select) {}
	virtual void draw() override;

private:
	const std::shared_ptr<ChElementLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

