#include "character_trace.h"

#include <qpolygonf>

CharacterTrace::CharacterTrace(const QPolygonF& poly)
{
	trace = poly;
}

CharacterTrace::~CharacterTrace()
{
}

QPointF CharacterTrace::get_point(int value)
{
	int t = value * (trace.size() - 1) / 100;
	return trace[t];
}
