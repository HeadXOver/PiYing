#pragma once

#include <qlist>
#include <memory>

class QOpenGLTexture;
class PointVector;

class Part final
{
public:
	Part(const QOpenGLTexture& texture, const PointVector& verts, const QList<unsigned int>& indices, bool isTexture);

private:
	const QOpenGLTexture& _texture;

	QList<unsigned int> _indices;
	std::unique_ptr<PointVector> _vert_texture;

	const PointVector& r_verts;
};