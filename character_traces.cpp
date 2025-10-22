#include "character_traces.h"

#include "character_trace.h"

#include <QPolygonF>

CharacterTraces::CharacterTraces()
{
}

CharacterTraces::~CharacterTraces()
{
	for (CharacterTrace* p : traces) delete p;
}
