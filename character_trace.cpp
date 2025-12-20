#include "character_trace.h"

#include <qpolygonf>

CharacterTrace::CharacterTrace(const QPolygonF& poly)
{
	trace = poly;
}

CharacterTrace::CharacterTrace(unsigned int index, const QPolygonF& poly, const QString& name) : _name(name)
{
	trace_by_index[index] = poly;
}

CharacterTrace::~CharacterTrace()
{
}

std::map<unsigned int, QPolygonF>& CharacterTrace::get_traces()
{
	return trace_by_index;
}

bool CharacterTrace::have_point(unsigned int index) const
{
	return trace_by_index.count(index);
}

void CharacterTrace::add_point(unsigned int index, const QPolygonF& poly)
{
	trace_by_index[index] = poly;
}

void CharacterTrace::set_current_value(int value)
{
	current_slider_value = value;
}

QPointF CharacterTrace::get_point(int value)
{
	int t = value * (trace.size() - 1) / 100;
	return trace[t];
}

const QString& CharacterTrace::name() const
{
	return _name;
}

int CharacterTrace::get_current_slider_value() const
{
	return current_slider_value;
}
