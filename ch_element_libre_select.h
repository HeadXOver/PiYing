#pragma once

#include <memory>

#include "ch_element_tool_behavior.h"

struct ChElementSelect;
struct GlVertReference;
class QPainter;
class QPointF;
class QPolygonF;
class PiYingGL;
class PointVector;
class PointVectorLayer;

struct ChElementLibreSelect final
{
public:
	ChElementLibreSelect(GlVertReference& glReference);

	void draw(QPainter& painter);
	void clickPos(const QPointF& mouse);
	void movePos(const QPointF& mouse);
	void releasePos(const QPointF& mouse);

	std::unique_ptr<ChElementSelect> chElementSelect;
	std::unique_ptr<QPolygonF> polygon;
	bool drawing = false;
	bool edit_skelen{ false };
};

///////////////////////////////////////////////////

class LibreSelectClick final : public ClickBehavior
{
public:
	LibreSelectClick(std::shared_ptr<ChElementLibreSelect> select) { libreSelect = select; }

	virtual void click(const QPointF& point) override;

private:
	std::shared_ptr<ChElementLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectMove final : public MouseMoveBehavior
{
public:
	LibreSelectMove(std::shared_ptr<ChElementLibreSelect> select) { libreSelect = select; }

	virtual void mouseMove(const QPointF& point) override;

private:
	std::shared_ptr<ChElementLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectRelease final : public ReleaseBehavior
{
public:
	LibreSelectRelease(std::shared_ptr<ChElementLibreSelect> select) { libreSelect = select; }

	virtual void release(const QPointF& point) override;

private:
	std::shared_ptr<ChElementLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectDelete final : public DeleteElementBehavior
{
public:
	LibreSelectDelete(std::shared_ptr<ChElementLibreSelect> select) { libreSelect = select; }


	virtual void deleteElement() override;

private:
	std::shared_ptr<ChElementLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectEscape final : public EscapeBehavior
{
public:
	LibreSelectEscape(std::shared_ptr<ChElementLibreSelect> select) { libreSelect = select; }


	virtual void escape() override;

private:
	std::shared_ptr<ChElementLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectDraw final : public DrawBehavior
{
public:
	LibreSelectDraw(std::shared_ptr<ChElementLibreSelect> select) { libreSelect = select; }
	virtual void draw(QPainter& painter) override;

private:
	std::shared_ptr<ChElementLibreSelect> libreSelect;
};


