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

CharacterTrace::CharacterTrace(const CharacterTrace& other) : 
	trace(other.trace),
	_name(other._name),
	current_slider_value(other.current_slider_value),
	trace_by_index(other.trace_by_index)
{
}

CharacterTrace::CharacterTrace(const CharacterTrace& other, unsigned int skew) :
	trace(other.trace),
	_name(other._name),
	current_slider_value(other.current_slider_value)
{
	for (auto& [index, poly] : other.trace_by_index) {
		trace_by_index[index + skew] = poly;
	}
}

CharacterTrace::~CharacterTrace()
{
}

std::unordered_map<unsigned int, QPolygonF>& CharacterTrace::get_traces()
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

void CharacterTrace::set_name(const QString& name)
{
	_name = name;
}

void CharacterTrace::add_skew(unsigned int skew)
{
	std::unordered_map<unsigned int, QPolygonF> newMap;
	newMap.swap(trace_by_index);  // 把原数据移到 newMap
	trace_by_index.clear();
	for (auto& [index, poly] : newMap) {
		trace_by_index[index + skew] = std::move(poly);
	}
}

const QString& CharacterTrace::name() const
{
	return _name;
}

int CharacterTrace::get_current_slider_value() const
{
	return current_slider_value;
}
