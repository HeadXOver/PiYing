#include "character_trace.h"

#include <qpolygonf>

CharacterTrace::CharacterTrace(unsigned int index, const QPolygonF& poly, const QString& name) : _name(name)
{
	// 记录相对于第一个点的偏移量
	trace_by_index[index] = poly.translated(-poly[0]);
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

const std::unordered_map<unsigned int, QPolygonF>& CharacterTrace::get_traces() const noexcept
{
	return trace_by_index;
}

bool CharacterTrace::have_point(unsigned int index) const noexcept
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

void CharacterTrace::set_current_value(int value) noexcept
{
	current_slider_value = value;
}

void CharacterTrace::set_name(const QString& name) noexcept
{
	_name = name;
}

void CharacterTrace::add_skew(unsigned int skew)
{
	std::unordered_map<unsigned int, QPolygonF> oldMap;
	oldMap.swap(trace_by_index);
	trace_by_index.reserve(oldMap.size());

	for (auto& [index, poly] : oldMap) {
		trace_by_index.emplace(index + skew, std::move(poly));
	}
}

const QString& CharacterTrace::name() const noexcept
{
	return _name;
}

int CharacterTrace::get_current_slider_value() const noexcept
{
	return current_slider_value;
}
