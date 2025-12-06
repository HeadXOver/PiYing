#pragma once

#include <qlist>
#include <vector>
#include <memory>

class QOpenGLTexture;
class PointVector;

class Part final
{
public:
	Part(const QOpenGLTexture& texture, const PointVector& verts, const std::vector<unsigned int>& triangleindices, const QList<unsigned int>& indices, bool isTexture);

private:
	void add_texture_part();
	void add_moved_part();

private:
	const QOpenGLTexture& _texture;

	QList<unsigned int> _indices;
	std::unique_ptr<PointVector> _vert_texture;

	const PointVector& r_verts;
	const std::vector<unsigned int>& r_triangleindices;
};