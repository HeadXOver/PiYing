#pragma once

class QMatrix4x4;

QMatrix4x4 getTrans(const QMatrix4x4& M);
QMatrix4x4 getRot(const QMatrix4x4& M);
QMatrix4x4 getScale(const QMatrix4x4& M);