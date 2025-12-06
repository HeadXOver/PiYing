#include "part.h"

#include "point_vector.h"

#include <qopengltexture>

Part::Part(const QOpenGLTexture& texture, const PointVector& verts, const QList<unsigned int>& indices, bool isTexture) : _texture(texture), r_verts(verts)
{
	_vert_texture = std::make_unique<PointVector>();
}
