#pragma once

#include <QPolygonF>
#include <map>
#include <qstring>

class QSlider;

class CharacterTrace
{
public:
    CharacterTrace(const QPolygonF& poly);
    CharacterTrace(unsigned int index, const QPolygonF& poly, const QString& name);
    ~CharacterTrace();

    std::map<unsigned int, QPolygonF>& get_traces();

    bool have_point(unsigned int index) const;

    void add_point(unsigned int index, const QPolygonF& poly);
    void set_current_value(int value);

    QPointF get_point(int value);

    const QString& name() const;

    int get_current_slider_value() const;

private:
    std::map<unsigned int, QPolygonF> trace_by_index;
    int current_slider_value{ 0 };
    QString _name;

    QPolygonF trace;
};

