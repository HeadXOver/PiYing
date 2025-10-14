#pragma once

#include "ChElementSelect.h"

class QPolygonF;

class ChElementLibreSelect final : public ChElementSelect
{
public:
	ChElementLibreSelect(int current, PiYingGL* gl);
	~ChElementLibreSelect();

protected:
	virtual void draw(QPainter& painter) override;
	virtual void clickPos(const QPointF& mouse) override;
	virtual void movePos(const QPointF& mouse) override;
	virtual void releasePos(const QPointF& mouse) override;

private:
	void addEnclosedPoints(const QPolygonF* const poly, const QList<QPointF>& points);

private:
	QPolygonF* polygon;
	bool drawing = false;
};

