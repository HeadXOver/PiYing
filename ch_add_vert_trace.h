#pragma once

#include "ch_element_tool_behavior.h"

#include <memory>
#include <qpolygon>

struct ChAddVertTrace final
{
	void click(const QPointF& mouse);
	void mouse_move(const QPointF& mouse);
	void release(const QPointF& mouse);
	void draw();

	int current_index = -1;
	bool presse_on_vert = false;
	QPolygonF polygon;
};

////////////////////////////////////////////////
 
PIYING_CLICK_BEHAVIOR(AddVertTraceClick, ChAddVertTrace)
PIYING_MOVEMOUSE_BEHAVIOR(AddVertTraceMove, ChAddVertTrace)

////////////////////////////////////////////////

class AddVertTraceRelease final : public ReleaseBehavior
{
public:
	AddVertTraceRelease(const std::shared_ptr<ChAddVertTrace>& add) : addTrace(add) {}
	virtual void release(const QPointF& mouse) override { addTrace->release(mouse); }

private:
	const std::shared_ptr<ChAddVertTrace> addTrace;
};

////////////////////////////////////////////////

class AddVertTraceDraw final : public DrawBehavior
{
public:
	AddVertTraceDraw(const std::shared_ptr<ChAddVertTrace>& add) : addTrace(add) {}
	virtual void draw() override { addTrace->draw(); }

private:
	const std::shared_ptr<ChAddVertTrace> addTrace;
};