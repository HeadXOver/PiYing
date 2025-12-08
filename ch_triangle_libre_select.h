#pragma once

#include <memory>
#include <qpolygonf>

#include "ch_element_tool_behavior.h"

struct ChTriangleSelect;
struct GlVertReference;
class QPainter;
class PiYingGL;
class PointVector;
class PointVectorLayer;

struct ChTriangleLibreSelect final
{
public:
	ChTriangleLibreSelect(GlVertReference& glReference);
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
	LibreSelectTriangleClick(std::shared_ptr<ChTriangleLibreSelect> select) { libreSelect = select; }

	virtual void click(const QPointF& point) override;

private:
	std::shared_ptr<ChTriangleLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectTriangleMove final : public MouseMoveBehavior
{
public:
	LibreSelectTriangleMove(std::shared_ptr<ChTriangleLibreSelect> select) { libreSelect = select; }

	virtual void mouseMove(const QPointF& point) override;

private:
	std::shared_ptr<ChTriangleLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectTriangleRelease final : public ReleaseBehavior
{
public:
	LibreSelectTriangleRelease(std::shared_ptr<ChTriangleLibreSelect> select) { libreSelect = select; }

	virtual void release(const QPointF& point) override;

private:
	std::shared_ptr<ChTriangleLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectTriangleDelete final : public DeleteElementBehavior
{
public:
	LibreSelectTriangleDelete(std::shared_ptr<ChTriangleLibreSelect> select) { libreSelect = select; }


	virtual void deleteElement() override;

private:
	std::shared_ptr<ChTriangleLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectTriangleEscape final : public EscapeBehavior
{
public:
	LibreSelectTriangleEscape(std::shared_ptr<ChTriangleLibreSelect> select) { libreSelect = select; }


	virtual void escape() override;

private:
	std::shared_ptr<ChTriangleLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectTriangleDraw final : public DrawBehavior
{
public:
	LibreSelectTriangleDraw(std::shared_ptr<ChTriangleLibreSelect> select) { libreSelect = select; }
	virtual void draw() override;

private:
	std::shared_ptr<ChTriangleLibreSelect> libreSelect;
};

///////////////////////////////////////////////////

class LibreSelectTriangleEnter final : public EnterBehavior
{
public:
	LibreSelectTriangleEnter(std::shared_ptr<ChTriangleLibreSelect> select) { libreSelect = select; }
	virtual void enter() override;

private:
	std::shared_ptr<ChTriangleLibreSelect> libreSelect;
};

