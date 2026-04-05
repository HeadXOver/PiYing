#pragma once

#include <qlist>
#include <qpointf>
#include <array>

class PointVectorLayer;

namespace piying {

	class SelectedPoints
	{
	public:
		const QList<unsigned int>& index() const noexcept { return selectedIndex; }
		const QList<QPointF>& vert() const noexcept { return lastVertPos; }

		size_t point_size() const noexcept { return selectedIndex.size(); }

		unsigned int* index_data() { return selectedIndex.data(); }

		unsigned int operator[](int index) const { return selectedIndex[index]; }
		const QPointF& getVert(int index) const { return lastVertPos[index]; }

		bool contains(unsigned int index) const noexcept { return selectedIndex.contains(index); }
		bool contains(const QPointF& vert) const noexcept { return lastVertPos.contains(vert); }

		bool isEmpty() const noexcept { return selectedIndex.isEmpty(); }

		void clear() { selectedIndex.clear(); lastVertPos.clear(); }
		void append(unsigned int ind);
		void removeLast() noexcept { selectedIndex.removeLast(); lastVertPos.removeLast(); }
		void affirmVert(const PointVectorLayer& fromLayer, bool edit_skelen);

	protected:
		QList<unsigned int> selectedIndex;
		QList<QPointF> lastVertPos;
	};

	class SelectedTriangle final : public SelectedPoints
	{
	public:
		size_t n_triangle() const noexcept { return selectedIndex.size() / 3; }

		bool contains(std::array<unsigned int, 3> triangle) const noexcept;

		void append(std::array<unsigned int, 3> ind);
		void remove_last_triangle() noexcept;
	};

}
