#include "part.h"

#include "point_vector.h"
#include "point_vector_layer.h"

#include "base_math.h"

#include "time_line_gl.h"
#include "global_objects.h"

#include <qopengltexture>
#include <qpointf>

Part::Part(
	QOpenGLTexture& texture,
	const QList<unsigned int>& indices,
	bool isTexture
) :
	_texture(texture)
{
	_vert_texture = std::make_unique<PointVector>();
	PointVectorLayer layer(*_vert_texture);

	std::vector<int> hashIndex(currentLayer->size(), -1);

	int eachIndex;
	int currentIndex = 0;

	const QPointF& firstPoint = (*currentLayer)[indices[0]];
	float top = firstPoint.y();
	float bottom = firstPoint.y();
	float left = firstPoint.x();
	float right = firstPoint.x();

	for (unsigned int i = 0; i < indices.size(); ++i) {
		eachIndex = hashIndex[indices[i]];
		if (eachIndex < 0) {
			hashIndex[indices[i]] = currentIndex;
			if (isTexture) {
				layer.push_back((*currentLayer)(indices[i]));

				top = cus::max(top, (*currentLayer)(indices[i]).y());
				bottom = cus::min(bottom, (*currentLayer)(indices[i]).y());
				left = cus::min(left, (*currentLayer)(indices[i]).x());
				right = cus::max(right, (*currentLayer)(indices[i]).x());
			}
			else {
				layer.push_back((*currentLayer)(indices[i]), (*currentLayer)[indices[i]]);

				top = cus::max(top, (*currentLayer)[indices[i]].y());
				bottom = cus::min(bottom, (*currentLayer)[indices[i]].y());
				left = cus::min(left, (*currentLayer)[indices[i]].x());
				right = cus::max(right, (*currentLayer)[indices[i]].x());
			}

			_indices.push_back(currentIndex++);
		}
		else {
			_indices.push_back(eachIndex);
		}
	}

	_x = (left + right) / 2.f;
	_y = (top + bottom) / 2.f;

	_height = top - bottom;
	_width = right - left;

	timelineGl->generate_vbo(*_vert_texture, _vbo);
	timelineGl->generate_ebo(_indices, _ebo);
}

float* Part::float_data() const
{
	return _vert_texture->data();
}

const unsigned int* Part::index_data() const
{
	return _indices.data();
}

size_t Part::float_size() const
{
	return _vert_texture->float_size();
}

size_t Part::index_size() const
{
	return _indices.size();
}

void Part::bind_texture()
{
	_texture.bind();
}

float Part::x() const
{
	return _x;
}

float Part::y() const
{
	return _y;
}

float Part::height() const
{
	return _height;
}

float Part::width() const
{
	return _width;
}

unsigned int Part::vbo() const
{
	return _vbo;
}

unsigned int Part::ebo() const
{
	return _ebo;
}