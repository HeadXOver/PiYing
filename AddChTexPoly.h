#pragma once

#include "ChElementTool.h"

class AddChTexPoly : public ChElementTool {

public:
	AddChTexPoly(int current, PiYingGL* gl) :ChElementTool(current, gl) {}

public:
	virtual void escape() override;
	virtual void enter() override;
	virtual void deleteElement() override;
	virtual void clickPos(const QPointF& mouse) override;
	virtual void movePos(const QPointF& mouse) override {}
	virtual void releasePos(const QPointF& mouse) override {}
	virtual void draw(QPainter& painter) override;

private:
	bool checkPointRepeat(const QPointF& point);

private:
	QList<QPointF> points;
	QList<int> index;
};

