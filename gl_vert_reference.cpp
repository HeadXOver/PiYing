#include "gl_vert_reference.h"
#include "piYingGL.h"
#include "point_vector.h"
#include "point_vector_layer.h"

#include <QPointF>

void GlVertReference::add_point_to_vert(const QPointF& p)
{
	pointLayer->push_back(p);
}

void GlVertReference::addChVert(const QPointF& point) 
{
	glIndex.push_back((unsigned int)pointLayer->size());
	pointLayer->push_back(point);
}

void GlVertReference::addTriangle(int index1, int index2, int index3)
{
	glIndex.push_back(index1);
	glIndex.push_back(index2);
	glIndex.push_back(index3);
	gl.update_ch_verts();
}

void GlVertReference::addTriangle(int index1, int index2, const QPointF& point3)
{
	glIndex.push_back(index1);
	glIndex.push_back(index2);
	addChVert(point3);
	gl.update_ch_verts();
}

void GlVertReference::addTriangle(int index1, const QPointF& point2, const QPointF& point3)
{
	glIndex.push_back(index1);
	addChVert(point2);
	addChVert(point3);
	gl.update_ch_verts();
}

void GlVertReference::addTriangle(const QPointF& point1, const QPointF& point2, const QPointF& point3)
{
	addChVert(point1);
	addChVert(point2);
	addChVert(point3);
	gl.update_ch_verts();
}

int GlVertReference::get_current_end() const
{
	return (int)pointLayer->size();
}

GlVertReference::GlVertReference(int current, PiYingGL& pygl) :
	glIndex(pygl.ref_characterTriangleIndices()[current]),
	gl(pygl)
{
	pointLayer = std::make_unique<PointVectorLayer>(pygl.ref_characterVerts(current));
}

GlVertReference::~GlVertReference()
{
}
