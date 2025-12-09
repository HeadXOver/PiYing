#include "piYingGL.h"

#include "global_objects.h"
#include "point_vector.h"
#include "point_vector_layer.h"

void PiYingGL::add_point_to_vert(const QPointF& p)
{
	currentLayer->push_back(p);
}

void PiYingGL::addChVert(const QPointF& point)
{
	currentIndex->push_back((unsigned int)currentLayer->size());
	currentLayer->push_back(point);
}

void PiYingGL::addTriangle(int index1, int index2, int index3)
{
	currentIndex->push_back(index1);
	currentIndex->push_back(index2);
	currentIndex->push_back(index3);
	update_ch_verts();
}

void PiYingGL::addTriangle(int index1, int index2, const QPointF& point3)
{
	currentIndex->push_back(index1);
	currentIndex->push_back(index2);
	addChVert(point3);
	update_ch_verts();
}

void PiYingGL::addTriangle(int index1, const QPointF& point2, const QPointF& point3)
{
	currentIndex->push_back(index1);
	addChVert(point2);
	addChVert(point3);
	update_ch_verts();
}

void PiYingGL::addTriangle(const QPointF& point1, const QPointF& point2, const QPointF& point3)
{
	addChVert(point1);
	addChVert(point2);
	addChVert(point3);
	update_ch_verts();
}
