#pragma once

#include "ChElementTool.h"

class AddTriangle : public ChElementTool {

public:
	AddTriangle(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v, int current) :ChElementTool(ind, v, current) {}

public:
	bool addIndex(unsigned int i);
	bool checkPointRepeat(const QPointF& point);
	void addVert(const QPointF& point);

	virtual void escape() override;
	virtual void enter() override {}
	virtual void deleteElement() override;
	virtual void clickPos(const QPointF& mouse, float viewScale) override;
	virtual void draw(QPainter& painter, PiYingGL* gl) override;

private:
	void addChVert(const QPointF& point);

private:
	QPointF first;
	QPointF second;

	unsigned int firstIndex = 0;
	unsigned int secondIndex = 0;

	bool vertThenInd = false;

	int numVert = 0;
	int numInd = 0;
};