#pragma once

#include <QPolygonF>

class CharacterTrace
{
public:
    CharacterTrace(const QPolygonF& poly);
    ~CharacterTrace();

private:
    QPolygonF trace;
    int current_position;
};

