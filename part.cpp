#include "part.h"

#include "point_vector.h"
#include "point_vector_layer.h"

#include "global_objects.h"

#include <qopengltexture>
#include <qpointf>

Part::Part(
	const QOpenGLTexture& texture, 
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

	for (unsigned int i = 0; i < indices.size(); ++i) {
		eachIndex = hashIndex[indices[i]];
		if (eachIndex < 0) {
			hashIndex[indices[i]] = currentIndex;
			if (isTexture) {
				layer.push_back((*currentLayer)(indices[i]));
			}
			else {
				layer.push_back((*currentLayer)(indices[i]), (*currentLayer)[indices[i]]);
			}
			_indices.push_back(currentIndex++);
		}
		else {
			_indices.push_back(eachIndex);
		}
	}
}
