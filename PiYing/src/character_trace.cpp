#include "character_trace.h"

#include <qpolygonf>
#include <piying_curve.h>

CharacterTrace::CharacterTrace(unsigned int index, const QPolygonF& poly, const QString& name) : _name(name)
{
	// 记录相对于第一个点的偏移量
	trace_by_index.emplace(index, poly.translated(-poly[0]));
}

CharacterTrace::CharacterTrace(unsigned int index, const QPointF& point, const QString& name) : _name(name)
{
	trace_by_index.emplace(index, point);
}

CharacterTrace::CharacterTrace(const CharacterTrace& other, unsigned int skew) :
	_name(other._name),
	current_slider_value(other.current_slider_value)
{
	for (auto [index, poly] : other.trace_by_index) {
		trace_by_index.emplace(index + skew, poly);
	}
}

CharacterTrace::~CharacterTrace()
{
}

void CharacterTrace::add_point(unsigned int index, const QPolygonF& poly)
{
	trace_by_index.emplace(index, poly.translated(-poly[0]));
}

void CharacterTrace::add_point(unsigned int index, const QPointF& point)
{
	trace_by_index.emplace(index, point);
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
	std::unordered_map<unsigned int, piying::Curve> oldMap;
	oldMap.swap(trace_by_index);
	trace_by_index.reserve(oldMap.size());

	for (auto& [index, poly] : oldMap) {
		trace_by_index.emplace(index + skew, std::move(poly));
	}
}