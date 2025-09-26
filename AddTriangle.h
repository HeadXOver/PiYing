#pragma once

#include "ChElementSelect.h"

class AddTriangle : public ChElementTool {
public:
	AddTriangle(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v) :ChElementTool(ind, v) {}

public:
	bool addIndex(unsigned int i);
	bool checkPointRepeat(const QPointF& point);
	void addVert(const QPointF& point);

	virtual void escape() override;
	virtual void clickPos(const QPointF& mouse, float viewScale, int currentVector) override;
	virtual QList<QPointF> getToDrawVert(int currentVector) override;

private:
	void addChVert(const QPointF& point, int currentVector);

private:
	QPointF first;
	QPointF second;

	unsigned int firstIndex = 0;
	unsigned int secondIndex = 0;

	bool vertThenInd = false;

	int numVert = 0;
	int numInd = 0;
};