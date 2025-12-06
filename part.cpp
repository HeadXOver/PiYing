#include "part.h"

#include "point_vector.h"

#include <qopengltexture>

Part::Part(QOpenGLTexture& texture, PointVector& pointVector, const std::vector<unsigned int>& indices, bool isTexture) : _texture(texture), r_verts(pointVector)
{
	_vert_texture = std::make_unique<PointVector>();
}
