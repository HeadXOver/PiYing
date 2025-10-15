#pragma once

#include "ChElementToolBehavior.h"

#include <memory>

struct GlVertReference;
class PiYingGL;

struct ChElementAddRound final 
{
	ChElementAddRound(GlVertReference* glVertReference);
	~ChElementAddRound();

	void click(const QPointF& mouse);
	void move(const QPointF& mouse);
	void release(const QPointF& mouse);
	void draw(QPainter& painter);

	QPointF* center;

	bool isPress = false;

	int radius = 0;

	GlVertReference* glVertReference;
};

////////////////////////////////////////////////

class AddRoundClick final : public ClickBehavior 
{
public:
	AddRoundClick(std::shared_ptr<ChElementAddRound> add) : addRound(add) {}
	virtual void click(const QPointF& mouse) override;

private:
	std::shared_ptr<ChElementAddRound> addRound;
};

////////////////////////////////////////////////

class AddRoundMove final : public MouseMoveBehavior
{
public:
	AddRoundMove(std::shared_ptr<ChElementAddRound> add) : addRound(add) {}
	virtual void mouseMove(const QPointF& mouse) override;

private:
	std::shared_ptr<ChElementAddRound> addRound;
};

////////////////////////////////////////////////

class AddRoundRelease final : public ReleaseBehavior
{
public:
	AddRoundRelease(std::shared_ptr<ChElementAddRound> add) : addRound(add) {}
	virtual void release(const QPointF& mouse) override;

private:
	std::shared_ptr<ChElementAddRound> addRound;
};

////////////////////////////////////////////////

class AddRoundDraw final : public DrawBehavior
{
public:
	AddRoundDraw(std::shared_ptr<ChElementAddRound> add) : addRound(add) {}
	virtual void draw(QPainter* painter) override;

private:
	std::shared_ptr<ChElementAddRound> addRound;
};