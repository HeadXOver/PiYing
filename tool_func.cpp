#include "piYingGL.h"

#include "point_vector_layer.h"
#include "static_gl_const.h"

void PiYingGL::add_point_to_vert(const QPointF& p)
{
	_currentLayer->push_back(p);
}

void PiYingGL::addChVert(const QPointF& point)
{
	_currentIndex->push_back((unsigned int)_currentLayer->size());
	_currentLayer->push_back(point);
}

void PiYingGL::addTriangle(int index1, int index2, int index3)
{
	_currentIndex->push_back(index1);
	_currentIndex->push_back(index2);
	_currentIndex->push_back(index3);
	update_ch_verts();
}

void PiYingGL::addTriangle(int index1, int index2, const QPointF& point3)
{
	_currentIndex->push_back(index1);
	_currentIndex->push_back(index2);
	addChVert(point3);
	update_ch_verts();
}

void PiYingGL::addTriangle(int index1, const QPointF& point2, const QPointF& point3)
{
	_currentIndex->push_back(index1);
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

void PiYingGL::generate_vao(unsigned int& vao, unsigned int vbo, unsigned int ebo)
{
	makeCurrent();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, FLOAT4, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, FLOAT4, DEBU_FLOAT2);
	glBindVertexArray(0);
	doneCurrent();
}

void PiYingGL::release_buffers(unsigned int vao) noexcept
{
	makeCurrent();
	glDeleteVertexArrays(1, &vao);
	doneCurrent();
}
