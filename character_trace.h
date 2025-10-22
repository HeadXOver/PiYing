#pragma once

class QPolygonF;

class CharacterTrace
{
public:
    CharacterTrace();
    ~CharacterTrace();

private:
    QPolygonF* trace;
    int index;
    int current_position;
};

