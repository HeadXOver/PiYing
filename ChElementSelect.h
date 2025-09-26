#pragma once

#include "ChElementTool.h"

class ChElementSelect : public ChElementTool
{
public:
	ChElementSelect(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v) :ChElementTool(ind, v) {}

public:
	virtual void escape() override { index.removeLast(); }
	virtual void deleteElement(int currentVector) override;
	virtual void clickPos(const QPointF& mouse, float viewScale, int currentVector) override;
	virtual QList<QPointF> getToDrawVert(int currentVector) override;

private:
	QList<unsigned int> index;
};