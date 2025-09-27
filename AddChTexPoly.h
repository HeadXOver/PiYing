#pragma once

#include "ChElementTool.h"

class AddChTexPoly : public ChElementTool {

public:
	AddChTexPoly(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v) :ChElementTool(ind, v) {}

public:
	virtual void escape() override;
	virtual void enter(int currentVector) override;
	virtual void deleteElement(int currentVector) override;
	virtual void clickPos(const QPointF& mouse, float viewScale, int currentVector) override;
	virtual QList<QPointF> getToDrawVert(int currentVector) override;

private:
	bool checkPointRepeat(const QPointF& point);
	void addTriangle(int v[3], int currentVector);

private:
	QList<QPointF> points;
	QList<int> index;
};

