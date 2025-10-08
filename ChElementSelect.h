#pragma once

#include "ChElementTool.h"

class ChElementSelect : public ChElementTool
{
public:
	ChElementSelect(int current, PiYingGL* gl) :ChElementTool(current, gl) {}

public:
	virtual void escape() override { if (index.size() > 0)index.removeLast(); }
	virtual void enter() override {}
	virtual void deleteElement() override;

protected:
	QList<unsigned int> index;
	QList<unsigned int> preIndex;
	QPointF lastPos;
	bool isPress = false;
};

class ChElementRectSelect : public ChElementSelect
{
public:
	ChElementRectSelect(int current, PiYingGL* gl) :ChElementSelect(current, gl) {}

public:
	virtual void draw(QPainter& painter) override;
	virtual void clickPos(const QPointF& mouse) override;
	virtual void movePos(const QPointF& mouse) override;
	virtual void releasePos(const QPointF& mouse) override;

private:
	QPointF rect;
	bool isDraw = false;
};