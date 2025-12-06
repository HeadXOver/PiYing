#pragma once

#include <vector>
#include <memory>

class QOpenGLTexture;
class PointVector;

class Part final
{
public:
	Part(QOpenGLTexture& texture, PointVector& verts, const std::vector<unsigned int>& indices, bool isTexture);

private:
	QOpenGLTexture& _texture;

	std::vector<unsigned int> _indices;
	std::unique_ptr<PointVector> _vert_texture;

	PointVector& r_verts;
};