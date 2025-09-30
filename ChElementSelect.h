#pragma once

#include "ChElementTool.h"

class ChElementSelect : public ChElementTool
{
public:
	ChElementSelect(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v, PiYingGL* gl) :ChElementTool(ind, v, gl) {}

public:
	virtual void escape() override { if (index.size() > 0)index.removeLast(); }
	virtual void enter() override {}
	virtual void deleteElement() override;
	virtual void clickPos(const QPointF& mouse) override;

protected:
	QList<unsigned int> index;
	QList<unsigned int> preIndex;
	QPointF lastPos;
	bool isPress = false;
};

class ChElementrrRectSelect : public ChElementSelect
{
public:
	ChElementrrRectSelect(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v, PiYingGL* gl) :ChElementSelect(ind, v, gl) {}

public:
	virtual void draw(QPainter& painter) override;
	virtual void movePos(const QPointF& mouse) override;
	virtual void releasePos(const QPointF& mouse) override;

private:
	QPointF rect;
	bool isDraw = false;
};