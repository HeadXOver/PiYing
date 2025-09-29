#pragma once

#include "ChElementTool.h"

class ChElementrrSelect : public ChElementTool
{
public:
	ChElementrrSelect(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v) :ChElementTool(ind, v) {}

public:
	virtual void escape() override { if (index.size() > 0)index.removeLast(); }
	virtual void enter(int currentVector) override {}
	virtual void deleteElement(int currentVector) override;
	virtual void clickPos(const QPointF& mouse, float viewScale, int currentVector) override;

protected:
	QList<unsigned int> index;
	QPointF lastPos;
};

class ChElementrrRectSelect : public ChElementrrSelect
{
public:
	ChElementrrRectSelect(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v) :ChElementrrSelect(ind, v) {}

public:
	virtual QList<QPointF> getToDrawVert(int currentVector) override;
};