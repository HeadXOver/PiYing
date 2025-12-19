#pragma once

#include <QPolygonF>
#include <map>
#include <qstring>

class CharacterTrace
{
public:
    CharacterTrace(const QPolygonF& poly);
    ~CharacterTrace();

    std::map<unsigned int, QPolygonF>& get_traces();

    bool have_point(unsigned int index) const;

    void add_point(unsigned int index, const QPolygonF& poly);

    QPointF get_point(int value);

private:
    std::map<unsigned int, QPolygonF> trace_by_index;
    QString _name;
    QPolygonF trace;
};

