#pragma once

#include <qlist>
#include <vector>
#include <memory>

class QOpenGLTexture;
class PointVector;

class Part final
{
public:
	Part(const QOpenGLTexture& texture, const QList<unsigned int>& indices, bool isTexture);

private:

private:
	const QOpenGLTexture& _texture;

	QList<unsigned int> _indices;
	std::unique_ptr<PointVector> _vert_texture;
};