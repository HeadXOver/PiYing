#pragma once

#include "CusInclude.h"

class PiYingGL;

class ChElementTool
{
public:
	ChElementTool(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v, int current) :glIndex(ind), glVert(v), currentVector(current) {}

public:
	virtual void escape() = 0;
	virtual void enter() = 0;
	virtual void deleteElement() = 0;
	virtual void clickPos(const QPointF& mouse, float viewScale) = 0;
	virtual void draw(QPainter& painter, PiYingGL* gl) = 0;

protected:
	QList<std::vector<float>>& glVert;
	QList<std::vector<unsigned int>>& glIndex;

	int currentVector;
};
