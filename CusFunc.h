#pragma once

#include <QMatrix4x4>
#include <QPointF>

class QListWidget;

QMatrix4x4 getTrans(const QMatrix4x4& M);
QMatrix4x4 getRot(const QMatrix4x4& M);
QMatrix4x4 getScale(const QMatrix4x4& M);

float angleBetweenPoint(const QPointF& p1, const QPointF& p2);
float angleBetweenPointDegree(const QPointF& p1, const QPointF& p2);

QPointF getRotatedPoint(const QPointF& p, float angle);
QPointF getRotatedPoint(float x, float y, float angle);
QString getUniquebgName(const QListWidget* list);

bool isInRect(const QPointF mouse, const QPointF& pos, int zone); 
bool isInRect(const QPointF mouse, const QPointF& pos, int width, int height);