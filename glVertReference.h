#pragma once

#include <vector>
#include <qlist>

class PiYingGL;
class QPointF;
class PointVectorLayer;

struct GlVertReference {
	void addPointToVert(const QPointF& p);
	void addChVert(const QPointF& point);
	void addTriangle(int index1, int index2, int index3);
	void addTriangle(int index1, int index2, const QPointF& point3);
	void addTriangle(int index1, const QPointF& point2, const QPointF& point3);
	void addTriangle(const QPointF& point1, const QPointF& point2, const QPointF& point3);

	int get_current_end() const;

	GlVertReference(int current, PiYingGL* piYingGL);
	PointVectorLayer* pointLayer;
	std::vector<unsigned int>& glIndex;
	PiYingGL* gl;
};