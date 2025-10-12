#include "ChElementLibreSelect.h"
#include "piYingGL.h"

void ChElementLibreSelect::draw(QPainter& painter)
{
	drawHandle(painter);

	if (!polygon.isEmpty()) {
		if (drawing) {
			painter.setPen(QPen(Qt::yellow, 1));

			auto mapper = [this](const QPointF& p) { return gl->mapViewProjMatrix(p); };

			QPolygonF screenPoly;
			screenPoly.reserve(polygon.size());
			std::transform(polygon.cbegin(), polygon.cend(),
				std::back_inserter(screenPoly),
				mapper);

			painter.drawPolyline(screenPoly);
			painter.drawLine(screenPoly.last(), screenPoly.first());
		}
	}

	for (int i = 0; i < index.size(); i++) {
		QPointF selectPoint = gl->mapViewProjMatrix(sVert[index[i]]);
		painter.setPen(QPen(Qt::black, 8));
		painter.drawPoint(selectPoint);
		painter.setPen(QPen(Qt::red, 6));
		painter.drawPoint(selectPoint);
	}
}

void ChElementLibreSelect::clickPos(const QPointF& mouse)
{
	isPress = true;

	polygon.clear();
	polygon << mouse;

	for (unsigned int i = 0; i < sVert.size(); i++) {
		if (QLineF(sVert[i], mouse).length() < 0.02f / gl->viewScale.value()) {
			if (!index.contains(i)) {
				if (!KeyboardStateWin::isCtrlHeld()) {
					index.clear();
				}
				index.append(i);
			}
			return;
		}
	}

	if (!KeyboardStateWin::isCtrlHeld()) {
		index.clear();
	}
}

void ChElementLibreSelect::movePos(const QPointF& mouse)
{
	if (isPress) {
		if (polygon.isEmpty() || polygon.last() != mouse) {
			polygon << mouse;
		}
		drawing = true;
	}
}

void ChElementLibreSelect::releasePos(const QPointF& mouse)
{
	if (drawing) {
		drawing = false;

		if (!polygon.isEmpty()) {
			polygon << polygon.first();
		}

		addEnclosedPoints(polygon, sVert);
	}
}

void ChElementLibreSelect::addEnclosedPoints(const QPolygonF& poly, const QList<QPointF>& points)
{
	for (unsigned int i = 0; i < points.size(); i++) {
		if (poly.containsPoint(points[i], Qt::OddEvenFill)) {
			index.append(i);
		}
	}
}
