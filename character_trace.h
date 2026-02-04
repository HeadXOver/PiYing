#pragma once

#include <QPolygonF>
#include <unordered_map>
#include <qstring>

class QSlider;

class CharacterTrace
{
public:
    CharacterTrace(unsigned int index, const QPolygonF& poly, const QString& name);
    CharacterTrace(const CharacterTrace& other);
    CharacterTrace(const CharacterTrace& other, unsigned int skew);
    ~CharacterTrace();

    std::unordered_map<unsigned int, QPolygonF>& get_traces();

    bool have_point(unsigned int index) const;

    void add_point(unsigned int index, const QPolygonF& poly);
    void set_current_value(int value);
    void set_name(const QString& name);
    void add_skew(unsigned int skew);

    const QString& name() const;

    int get_current_slider_value() const;

private:
    std::unordered_map<unsigned int, QPolygonF> trace_by_index;
    int current_slider_value{ 0 };
    QString _name;

    QPolygonF trace;
};

