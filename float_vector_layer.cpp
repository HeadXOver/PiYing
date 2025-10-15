#include "float_vector_layer.h"

size_t FloatVectorLayer::size() const {
	return layer.size();
}

float* FloatVectorLayer::data() const {
	return layer.data();
}