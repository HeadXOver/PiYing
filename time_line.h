#pragma once

class QVector4D;

struct ScaleTrans;

class Timeline
{
public:
	Timeline(float lenth);

public:
	float lenth() const;
	QVector4D get_transform(int line, const ScaleTrans* scaleTrans) const;

private:
	float _lenth{ 0.f };
	float _front{ 0.f };
	float _back{ 0.f };
};