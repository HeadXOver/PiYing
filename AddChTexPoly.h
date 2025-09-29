#pragma once

#include "ChElementTool.h"

class AddChTexPoly : public ChElementTool {

public:
	AddChTexPoly(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v, int current) :ChElementTool(ind, v, current) {}

public:
	virtual void escape() override;
	virtual void enter() override;
	virtual void deleteElement() override;
	virtual void clickPos(const QPointF& mouse, float viewScale) override;
	virtual void draw(QPainter& painter, PiYingGL* gl) override;

private:
	bool checkPointRepeat(const QPointF& point);

private:
	QList<QPointF> points;
	QList<int> index;
};

