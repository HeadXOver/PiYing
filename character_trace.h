#pragma once

#include <QPolygonF>
#include <unordered_map>
#include <qstring>

class QSlider;

class CharacterTrace
{
public:
    CharacterTrace(unsigned int index, const QPolygonF& poly, const QString& name);
    CharacterTrace(const CharacterTrace& other) = default;
    CharacterTrace(const CharacterTrace& other, unsigned int skew);
    ~CharacterTrace();

    const std::unordered_map<unsigned int, QPolygonF>& get_traces() const noexcept;

    bool have_point(unsigned int index) const noexcept;

    void add_point(unsigned int index, const QPolygonF& poly);
    void set_current_value(int value) noexcept;
    void set_name(const QString& name) noexcept;
    void add_skew(unsigned int skew);

    const QString& name() const noexcept;

    int get_current_slider_value() const noexcept;

private:
    std::unordered_map<unsigned int, QPolygonF> trace_by_index;
    int current_slider_value{ 0 };
    QString _name;

    QPolygonF trace;
};

