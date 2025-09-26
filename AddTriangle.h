#pragma once

#include "ChElementSelect.h"

class AddTriangle : public ChElementTool {
public:
	AddTriangle(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v) :ChElementTool(ind, v) {}

public:
	virtual bool addIndex(unsigned int i) override;
	virtual bool checkPointRepeat(const QPointF& point) override;
	virtual void addVert(const QPointF& point) override;
	virtual unsigned int getIndex(int i) override { return i == 0 ? firstIndex : secondIndex; }
	virtual QPointF getVert(int i) override { return i == 0 ? first : second; }

private:
	QPointF first;
	QPointF second;

	unsigned int firstIndex = 0;
	unsigned int secondIndex = 0;
};