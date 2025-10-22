#include "character_trace.h"

#include <qpolygonf>

CharacterTrace::CharacterTrace(const QPolygonF& poly)
{
	trace = poly;
}

CharacterTrace::~CharacterTrace()
{
}
