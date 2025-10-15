#include "glVertReference.h"
#include "piYingGL.h"
#include "point_vector.h"

#include <QPointF>

void GlVertReference::addPointToVert(const QPointF& p)
{
	glVert.push_back(p);
	uvVert.push_back(p);
}

void GlVertReference::addChVert(const QPointF& point) 
{
	glIndex.push_back((unsigned int)uvVert.size());
	addPointToVert(point);
}

GlVertReference::GlVertReference(int current, PiYingGL* pygl) :
	glIndex(pygl->ref_characterTriangleIndices()[current]),
	glVert(pygl->ref_characterVerts(current)),
	uvVert(pygl->ref_characterUVVerts(current)),
	gl(pygl)
{
}