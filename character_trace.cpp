#include "character_trace.h"

#include <qpolygonf>

CharacterTrace::CharacterTrace()
{
	trace = new QPolygonF();
}

CharacterTrace::~CharacterTrace()
{
	delete trace;
}
