#include "piYingGL.h"

#include "character_trace.h"
#include "slide_applier.h"
#include "point_vector_layer.h"

void PiYingGL::controlSlide(int id, int value)
{
	const std::map<int, std::unique_ptr<CharacterTrace>>& traces = slide_applier.get_traces(id);

	PointVectorLayer layer(*characterVerts[getCurrentChRow()]);

	for (const auto& [key, val] : traces) {
		layer.set_point(true, key, val->get_point(value));
	}

	update_ch_verts();
	update();
}