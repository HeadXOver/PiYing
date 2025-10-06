#pragma once

#include "ChElementSelect.h"

class ChElementLibreSelect : public ChElementSelect
{
public:
	ChElementLibreSelect(QList<std::vector<unsigned int>>& ind, QList<std::vector<float>>& v, PiYingGL* gl) :ChElementSelect(ind, v, gl) {}

protected:
	virtual void draw(QPainter& painter) override;
	virtual void clickPos(const QPointF& mouse) override;
	virtual void movePos(const QPointF& mouse) override;
	virtual void releasePos(const QPointF& mouse) override;

private:
	void addEnclosedPoints(const QPolygonF& poly, const std::vector<float>& points);

private:
	QPolygonF polygon;
	bool drawing = false;
};

