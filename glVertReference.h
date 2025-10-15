#pragma once

#include <vector>
#include <qlist>

class PiYingGL;
class QPointF;
class PointVector;

struct GlVertReference {
	void addPointToVert(const QPointF& p);
	void addChVert(const QPointF& point);

	GlVertReference(int current, PiYingGL* piYingGL);
	PointVector& glVert;
	PointVector& uvVert;
	std::vector<unsigned int>& glIndex;
	PiYingGL* gl;
};