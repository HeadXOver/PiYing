#include "glVertReference.h"
#include "piYingGL.h"
#include "point_vector.h"

#include <QPointF>

void GlVertReference::addPointToVert(const QPointF& p)
{
	glVert->push_back(p);
	sVert.push_back(p);
}

void GlVertReference::addChVert(const QPointF& point) 
{
	glIndex.push_back((unsigned int)sVert.size());
	addPointToVert(point);
}

GlVertReference::GlVertReference(int current, PiYingGL* pygl) :
	glIndex(pygl->ref_characterTriangleIndices()[current]),
	glVert(pygl->ref_characterVerts()[current]),
	sVert(pygl->ref_characterDrawVerts()[current]),
	gl(pygl)
{
}