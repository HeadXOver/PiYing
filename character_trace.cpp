#include "character_trace.h"

#include <qpolygonf>

CharacterTrace::CharacterTrace(unsigned int index, const QPolygonF& poly, const QString& name) : _name(name)
{
	QPointF origin = poly[0];
	QPolygonF screenPoly;
	screenPoly.reserve(poly.size());
	std::transform(poly.cbegin(), poly.cend(),
		std::back_inserter(screenPoly),
		[origin](const QPointF& p) { return p - origin; }
	);

	trace_by_index[index] = screenPoly;
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
	QPointF origin = poly[0];
	QPolygonF screenPoly;
	screenPoly.reserve(poly.size());
	std::transform(poly.cbegin(), poly.cend(),
		std::back_inserter(screenPoly),
		[origin](const QPointF& p) { return p - origin; }
	);

	trace_by_index[index] = screenPoly;
}

void CharacterTrace::set_current_value(int value)
{
	current_slider_value = value;
}

const QString& CharacterTrace::name() const
{
	return _name;
}

int CharacterTrace::get_current_slider_value() const
{
	return current_slider_value;
}
