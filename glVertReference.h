#pragma once

#include <vector>
#include <qlist>

class PiYingGL;
class QPointF;

struct GlVertReference {
	void addPointToVert(const QPointF& p);
	void addChVert(const QPointF& point);

	GlVertReference(int current, PiYingGL* piYingGL);
	std::vector<float>& glVert;
	std::vector<unsigned int>& glIndex;
	QList<QPointF>& sVert;
	PiYingGL* gl;
};