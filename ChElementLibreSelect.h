#pragma once

#include "ChElementSelect.h"

class ChElementLibreSelect : public ChElementSelect
{
public:
	ChElementLibreSelect(int current, PiYingGL* gl) :ChElementSelect(current, gl) {}

protected:
	virtual void draw(QPainter& painter) override;
	virtual void clickPos(const QPointF& mouse) override;
	virtual void movePos(const QPointF& mouse) override;
	virtual void releasePos(const QPointF& mouse) override;
	virtual void keyPress(int key) override;

private:
	void addEnclosedPoints(const QPolygonF& poly, const QList<QPointF>& points);

private:
	QPolygonF polygon;
	bool drawing = false;
};

