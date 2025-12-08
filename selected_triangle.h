#pragma once

#pragma once

#include <qlist>
#include <qpointf>

class PointVectorLayer;

class SelectedTriangle
{
public:
	SelectedTriangle(bool first, PointVectorLayer& vert) : sVert(vert) {}

	const QList<unsigned int>& index_list() const { return selectedIndex; }
	const QList<QPointF>& vert() const { return lastVertPos; }

	int size() const { return selectedIndex.size(); }
	int n_triangle() const { return selectedIndex.size() / 3; }

	unsigned int* index_data() { return selectedIndex.data(); }

	unsigned int operator[](int index) const { return selectedIndex[index]; }
	const QPointF& getVert(int index) const;

	bool contains(unsigned int index) const { return selectedIndex.contains(index); }
	bool contains(const unsigned int* triangle) const;
	bool contains(const QPointF& vert) const { return lastVertPos.contains(vert); }

	void clear() { selectedIndex.clear(); lastVertPos.clear(); }
	void append(const unsigned int* ind);
	void remove_last_triangle();
	void affirmVert(bool edit_skelen);

private:
	QList<unsigned int> selectedIndex;
	QList<QPointF> lastVertPos;

	PointVectorLayer& sVert;
};
