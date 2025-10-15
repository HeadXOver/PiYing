#include "glVertReference.h"
#include "piYingGL.h"
#include "point_vector.h"
#include "point_vector_layer.h"

#include <QPointF>

void GlVertReference::addPointToVert(const QPointF& p)
{
	pointLayer->push_back(p);
}

void GlVertReference::addChVert(const QPointF& point) 
{
	glIndex.push_back((unsigned int)pointLayer->size());
	addPointToVert(point);
}

GlVertReference::GlVertReference(int current, PiYingGL* pygl) :
	glIndex(pygl->ref_characterTriangleIndices()[current]),
	pointLayer(new PointVectorLayer(pygl->ref_characterVerts(current))),
	gl(pygl)
{
}