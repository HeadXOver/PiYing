#pragma once

#include <qpointf>
#include <memory>

#include "ChElementToolBehavior.h"

class PiYingGL;
struct GlVertReference;

struct ChElementAddRound
{
	ChElementAddRound(GlVertReference* glReference);

	void click(const QPointF& mouse);
	void move(const QPointF& mouse);
	void release(const QPointF& mouse);

	GlVertReference* glVertReference;
};

//////////////////////////////////////////////////////

class AddRoundClick : public ClickBehavior {
public:
	AddRoundClick(std::shared_ptr<ChElementAddRound> add) : addChElementAddRound(add) {}

	virtual void click(const QPointF& mouse) override;

private:
	std::shared_ptr<ChElementAddRound> addChElementAddRound;
};

//////////////////////////////////////////////////////

class AddRoundMove : public MouseMoveBehavior {
public:
	AddRoundMove(std::shared_ptr<ChElementAddRound> add) : addChElementAddRound(add) {}

	virtual void mouseMove(const QPointF& mouse) override;

private:
	std::shared_ptr<ChElementAddRound> addChElementAddRound;
};

//////////////////////////////////////////////////////

class AddRoundRelease : public ReleaseBehavior {
public:
	AddRoundRelease(std::shared_ptr<ChElementAddRound> add) : addChElementAddRound(add) {}

	virtual void release(const QPointF& mouse) override;

private:
	std::shared_ptr<ChElementAddRound> addChElementAddRound;
};

