#pragma once

#include "CusInclude.h"

class ChElementTool
{
public:
	ChElementTool(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v) :glIndex(ind), glVert(v) {}

public:
	virtual void escape() = 0;
	virtual void enter(int currentVector) = 0;
	virtual void deleteElement(int currentVector) = 0;
	virtual void clickPos(const QPointF& mouse, float viewScale, int currentVector) = 0;
	virtual QList<QPointF> getToDrawVert(int currentVector) = 0;

protected:
	QList<std::vector<float>>& glVert;
	QList<std::vector<unsigned int>>& glIndex;
};
