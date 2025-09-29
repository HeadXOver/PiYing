#pragma once

#include "ChElementTool.h"

class AddChTexPoly : public ChElementTool {

public:
	AddChTexPoly(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v, PiYingGL* gl) :ChElementTool(ind, v, gl) {}

public:
	virtual void escape() override;
	virtual void enter() override;
	virtual void deleteElement() override;
	virtual void clickPos(const QPointF& mouse) override;
	virtual void movePos(const QPointF& mouse) override {}
	virtual void releasePos() override {}
	virtual void draw(QPainter& painter) override;

private:
	bool checkPointRepeat(const QPointF& point);

private:
	QList<QPointF> points;
	QList<int> index;
};

