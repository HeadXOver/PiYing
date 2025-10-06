#include "ChElementLibreSelect.h"
#include "piYingGL.h"

void ChElementLibreSelect::draw(QPainter& painter)
{
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
		int ind = index[i];
		ind += ind;
		QPointF selectPoint = gl->mapViewProjMatrix(QPointF(glVert[currentVector][ind], glVert[currentVector][ind + 1]));
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

	for (unsigned int i = 0; i < glVert[currentVector].size() / 2; i++) {
		QPointF readyPoint(glVert[currentVector][i + i], glVert[currentVector][i + i + 1]);
		if (QLineF(readyPoint, mouse).length() < 0.02f / gl->viewScale.value()) {
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

		addEnclosedPoints(polygon, glVert[currentVector]);
	}
}

void ChElementLibreSelect::addEnclosedPoints(const QPolygonF& poly, const std::vector<float>& points)
{
	for (int i = 0; i < points.size(); i += 2) {
		if (poly.containsPoint(QPointF(points[i], points[i + 1]), Qt::OddEvenFill)) {
			index.append(i / 2);
		}
	}
}
