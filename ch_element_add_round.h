#pragma once

#include "ch_element_tool_behavior.h"

#include <memory>
#include <qpointf>

struct ChElementAddRound final 
{
	void click(const QPointF& mouse);
	void move(const QPointF& mouse);
	void release(const QPointF& mouse);
	void draw();

	void addRoundPoly(const int edgeCount);

	QPointF center;
	QPointF gl_center;
	QPointF current_cursor;
		
	bool isPress{ false };

	int radius{ 0 };
	int init_angle{ 0 };
};

////////////////////////////////////////////////

class AddRoundClick final : public ClickBehavior 
{
public:
	AddRoundClick(const std::shared_ptr<ChElementAddRound>& add) : addRound(add) {}
	virtual void click(const QPointF& mouse) override;

private:
	const std::shared_ptr<ChElementAddRound> addRound;
};

////////////////////////////////////////////////

class AddRoundMove final : public MouseMoveBehavior
{
public:
	AddRoundMove(const std::shared_ptr<ChElementAddRound>& add) : addRound(add) {}
	virtual void mouse_move(const QPointF& mouse) override;

private:
	const std::shared_ptr<ChElementAddRound> addRound;
};

////////////////////////////////////////////////

class AddRoundRelease final : public ReleaseBehavior
{
public:
	AddRoundRelease(const std::shared_ptr<ChElementAddRound>& add) : addRound(add) {}
	virtual void release(const QPointF& mouse) override;

private:
	const std::shared_ptr<ChElementAddRound> addRound;
};

////////////////////////////////////////////////

class AddRoundDraw final : public DrawBehavior
{
public:
	AddRoundDraw(const std::shared_ptr<ChElementAddRound>& add) : addRound(add) {}
	virtual void draw() override;

private:
	const std::shared_ptr<ChElementAddRound> addRound;
};