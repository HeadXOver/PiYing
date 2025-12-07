#include "part.h"

#include "point_vector.h"
#include "point_vector_layer.h"

#include <qopengltexture>

Part::Part(
	const QOpenGLTexture& texture, 
	const PointVector& verts, 
	const std::vector<unsigned int>& triangleindices, 
	const QList<unsigned int>& indices, 
	bool isTexture
) :
	_texture(texture), 
	r_verts(verts),
	r_triangleindices(triangleindices),
	_indices(indices)
{
	_vert_texture = std::make_unique<PointVector>();

	if (isTexture) add_texture_part();
	else add_moved_part();
}

void Part::add_texture_part()
{

}

void Part::add_moved_part()
{
}
