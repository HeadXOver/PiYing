#pragma once

#include "CusInclude.h"

class PiYingGL;

class ChElementTool
{
public:
	ChElementTool(int current, PiYingGL* pygl);

public:
	virtual void escape() = 0;
	virtual void enter() = 0;
	virtual void deleteElement() = 0;
	virtual void keyPress(int key) = 0;
	virtual void clickPos(const QPointF& mouse) = 0;
	virtual void movePos(const QPointF& mouse) = 0;
	virtual void releasePos(const QPointF& mouse) = 0;
	virtual void draw(QPainter& painter) = 0;

	void addPointToVert(const QPointF& p);

protected:
	std::vector<float>& glVert;
	std::vector<unsigned int>& glIndex;
	QList<QPointF>& sVert;
	PiYingGL* gl;
};
