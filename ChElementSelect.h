#pragma once

#include "CusInclude.h"

class ChElementTool
{
public:
	ChElementTool(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v) :glIndex(ind), glVert(v), numInd(0), numVert(0) {}

public:
	int getNumVert() { return numVert; }
	int getNumInd() { return numInd; }
	void setNumVert(int n) { numVert = n; }
	void setNumInd(int n) { numInd = n; }

	virtual bool addIndex(unsigned int i) = 0; 
	virtual bool checkPointRepeat(const QPointF& point) = 0;
	virtual void addVert(const QPointF& point) = 0;
	virtual void escape() = 0;
	virtual unsigned int getIndex(int i) = 0;
	virtual QPointF getVert(int i) = 0;

protected:
	QList<std::vector<float>>& glVert;
	QList<std::vector<unsigned int>>& glIndex;

	int numVert;
	int numInd;
};


class ChElementSelect : public ChElementTool
{
public:
	ChElementSelect(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v) :ChElementTool(ind, v) {}

public:
	virtual bool addIndex(unsigned int i) override { return false; }
	virtual bool checkPointRepeat(const QPointF& point)override { return false; }
	virtual void addVert(const QPointF& point) override {}
	virtual void escape() override {}
	virtual unsigned int getIndex(int i) override { return index[i]; }
	virtual QPointF getVert(int i) override { return QPointF(); }

private:
	QList<unsigned int> index;
};
