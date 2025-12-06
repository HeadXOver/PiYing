#include "part.h"

#include "point_vector.h"

#include <qopengltexture>

Part::Part(QOpenGLTexture& texture) : _texture(texture)
{
	_vert_texture = std::make_unique<PointVector>();
}
