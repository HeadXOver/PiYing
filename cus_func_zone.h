#pragma once

class QPointF;

bool isInRect(const QPointF& mouse, const QPointF& pos, int zone);
bool isInRect(const QPointF& mouse, const QPointF& pos, int width, int height);