#include "gl_vert_reference.h"
#include "piYingGL.h"
#include "global_objects.h"
#include "point_vector.h"
#include "point_vector_layer.h"

#include <QPointF>

void GlVertReference::add_point_to_vert(const QPointF& p)
{
	currentLayer->push_back(p);
}

void GlVertReference::addChVert(const QPointF& point) 
{
	glIndex.push_back((unsigned int)currentLayer->size());
	currentLayer->push_back(point);
}

void GlVertReference::addTriangle(int index1, int index2, int index3)
{
	glIndex.push_back(index1);
	glIndex.push_back(index2);
	glIndex.push_back(index3);
	piYingGL->update_ch_verts();
}

void GlVertReference::addTriangle(int index1, int index2, const QPointF& point3)
{
	glIndex.push_back(index1);
	glIndex.push_back(index2);
	addChVert(point3);
	piYingGL->update_ch_verts();
}

void GlVertReference::addTriangle(int index1, const QPointF& point2, const QPointF& point3)
{
	glIndex.push_back(index1);
	addChVert(point2);
	addChVert(point3);
	piYingGL->update_ch_verts();
}

void GlVertReference::addTriangle(const QPointF& point1, const QPointF& point2, const QPointF& point3)
{
	addChVert(point1);
	addChVert(point2);
	addChVert(point3);
	piYingGL->update_ch_verts();
}

GlVertReference::GlVertReference(int current, PiYingGL& pygl) :
	glIndex(pygl.ref_characterTriangleIndices()[current])
{
}

GlVertReference::~GlVertReference()
{
}
