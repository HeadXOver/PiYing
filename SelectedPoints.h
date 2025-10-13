#pragma once

#include <QList>
#include <vector>
#include <QPoint>

class SelectedPoints
{
public:
	SelectedPoints(QList<QPointF>& vert) : sVert(vert) {}

	const QList<unsigned int>& index() const { return selectedIndex; }
	const QList<QPointF>& vert() const { return lastVertPos; }

	int size() const { return selectedIndex.size(); }

	unsigned int operator[](int index) const { return selectedIndex[index]; }
	const QPointF& getVert(int index) const { return lastVertPos[index]; }

	bool contains(unsigned int index) const { return selectedIndex.contains(index); }
	bool contains(const QPointF& vert) const { return lastVertPos.contains(vert); }

	void clear() { selectedIndex.clear(); lastVertPos.clear(); }
	void append(unsigned int ind) { selectedIndex.append(ind); lastVertPos.append(sVert[ind]); }
	void removeLast() { selectedIndex.removeLast(); lastVertPos.removeLast(); }
	void affirmVert();

private:
	QList<unsigned int> selectedIndex;
	QList<QPointF> lastVertPos;

	QList<QPointF>& sVert;
};

