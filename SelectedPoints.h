#pragma once

#include <qlist>
#include <qpointf>

class PointVector;

class SelectedPoints
{
public:
	SelectedPoints(PointVector& vert) : sVert(vert) {}

	const QList<unsigned int>& index() const { return selectedIndex; }
	const QList<QPointF>& vert() const { return lastVertPos; }

	int size() const { return selectedIndex.size(); }

	unsigned int operator[](int index) const { return selectedIndex[index]; }
	const QPointF& getVert(int index) const;

	bool contains(unsigned int index) const { return selectedIndex.contains(index); }
	bool contains(const QPointF& vert) const { return lastVertPos.contains(vert); }

	void clear() { selectedIndex.clear(); lastVertPos.clear(); }
	void append(unsigned int ind);
	void removeLast() { selectedIndex.removeLast(); lastVertPos.removeLast(); }
	void affirmVert();

private:
	QList<unsigned int> selectedIndex;
	QList<QPointF> lastVertPos;

	PointVector& sVert;
};

