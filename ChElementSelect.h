#pragma once

#include "ChElementTool.h"

class ChElementrrSelect : public ChElementTool
{
public:
	ChElementrrSelect(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v, int current) :ChElementTool(ind, v, current) {}

public:
	virtual void escape() override { if (index.size() > 0)index.removeLast(); }
	virtual void enter() override {}
	virtual void deleteElement() override;
	virtual void clickPos(const QPointF& mouse, float viewScale) override;

protected:
	QList<unsigned int> index;
	QPointF lastPos;
};

class ChElementrrRectSelect : public ChElementrrSelect
{
public:
	ChElementrrRectSelect(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v, int current) :ChElementrrSelect(ind, v, current) {}

public:
	virtual void draw(QPainter& painter, PiYingGL* gl) override;
};