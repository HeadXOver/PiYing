#pragma once

#include <QPolygonF>

class CharacterTrace
{
public:
    CharacterTrace(const QPolygonF& poly);
    ~CharacterTrace();

    QPointF get_point(int value);

private:
    QPolygonF trace;
};

