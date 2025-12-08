#pragma once

#include "ch_element_tool_behavior.h"

#include <memory>
#include <qpolygon>

struct GlVertReference;
class PiYingGL;
class QPolygonF;

struct ChAddVertTrace final
{
	ChAddVertTrace(GlVertReference& glVertReference);
	~ChAddVertTrace();

	void click(const QPointF& mouse);
	void move(const QPointF& mouse);
	void release(const QPointF& mouse);
	void draw();

	int current_index = -1;
	bool presse_on_vert = false;
	QPolygonF polygon;

	GlVertReference& glVertReference;
};

////////////////////////////////////////////////

class AddVertTraceClick final : public ClickBehavior
{
public:
	AddVertTraceClick(std::shared_ptr<ChAddVertTrace> add) : addTrace(add) {}
	virtual void click(const QPointF& mouse) override;

private:
	std::shared_ptr<ChAddVertTrace> addTrace;
};

////////////////////////////////////////////////

class AddVertTraceMove final : public MouseMoveBehavior
{
public:
	AddVertTraceMove(std::shared_ptr<ChAddVertTrace> add) : addTrace(add) {}
	virtual void mouseMove(const QPointF& mouse) override;

private:
	std::shared_ptr<ChAddVertTrace> addTrace;
};

////////////////////////////////////////////////

class AddVertTraceRelease final : public ReleaseBehavior
{
public:
	AddVertTraceRelease(std::shared_ptr<ChAddVertTrace> add) : addTrace(add) {}
	virtual void release(const QPointF& mouse) override;

private:
	std::shared_ptr<ChAddVertTrace> addTrace;
};

////////////////////////////////////////////////

class AddVertTraceDraw final : public DrawBehavior
{
public:
	AddVertTraceDraw(std::shared_ptr<ChAddVertTrace> add) : addTrace(add) {}
	virtual void draw() override;

private:
	std::shared_ptr<ChAddVertTrace> addTrace;
};