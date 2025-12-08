#pragma once

#include <vector>
#include <qlist>
#include <memory>

class PiYingGL;
class QPointF;
class PointVectorLayer;

struct GlVertReference final {
	GlVertReference(int current, PiYingGL& piYingGL);
	~GlVertReference();

	void add_point_to_vert(const QPointF& p);
	void addTriangle(int index1, int index2, int index3);
	void addTriangle(int index1, int index2, const QPointF& point3);
	void addTriangle(int index1, const QPointF& point2, const QPointF& point3);
	void addTriangle(const QPointF& point1, const QPointF& point2, const QPointF& point3);

	int get_current_end() const;

	std::unique_ptr<PointVectorLayer> pointLayer;
	std::vector<unsigned int>& glIndex;

private:
	void addChVert(const QPointF& point);
};