#pragma once

#include <qlist>
#include <qpointf>

class PointVectorLayer;

class SelectedPoints
{
public:
	SelectedPoints(bool first, PointVectorLayer& vert) : sVert(vert) {}

	const QList<unsigned int>& index() const noexcept { return selectedIndex; }
	const QList<QPointF>& vert() const noexcept { return lastVertPos; }

	int size() const noexcept { return static_cast<int>(selectedIndex.size()); }

	unsigned int operator[](int index) const { return selectedIndex[index]; }
	const QPointF& getVert(int index) const;

	bool contains(unsigned int index) const noexcept { return selectedIndex.contains(index); }
	bool contains(const QPointF& vert) const noexcept { return lastVertPos.contains(vert); }

	void clear() { selectedIndex.clear(); lastVertPos.clear(); }
	void append(unsigned int ind);
	void removeLast() noexcept { selectedIndex.removeLast(); lastVertPos.removeLast(); }
	void affirmVert(bool edit_skelen);

private:
	QList<unsigned int> selectedIndex;
	QList<QPointF> lastVertPos;

	PointVectorLayer& sVert;
};

