#pragma once

#include <vector>
#include <qlist>

class PiYingGL;
class QPointF;
class PointVectorLayer;

struct GlVertReference {
	void addPointToVert(const QPointF& p);
	void addChVert(const QPointF& point);

	GlVertReference(int current, PiYingGL* piYingGL);
	PointVectorLayer* pointLayer;
	std::vector<unsigned int>& glIndex;
	PiYingGL* gl;
};