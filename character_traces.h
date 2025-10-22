#pragma once

#include <QList>

class QPolygonF;
class CharacterTrace;

class CharacterTraces
{
public:
    CharacterTraces();
    ~CharacterTraces();

private:
    QList<CharacterTrace*> traces;
};

