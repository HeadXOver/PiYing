#include "ChElementLibreSelect.h"
#include "piYingGL.h"
#include "SelectedPoints.h"

#include <qpolygonf>
#include <qpainter>

ChElementLibreSelect::ChElementLibreSelect(int current, PiYingGL* gl) :ChElementSelect(current, gl) {
	polygon = new QPolygonF();
}

ChElementLibreSelect::~ChElementLibreSelect()
{
	delete polygon;
}

void ChElementLibreSelect::draw(QPainter& painter)
{
	drawHandle(painter);

	if (!polygon->isEmpty()) {
		if (drawing) {
			painter.setPen(QPen(Qt::yellow, 1));

			auto mapper = [this](const QPointF& p) { return gl->mapViewProjMatrix(p); };

			QPolygonF screenPoly;
			screenPoly.reserve(polygon->size());
			std::transform(polygon->cbegin(), polygon->cend(),
				std::back_inserter(screenPoly),
				mapper
			);

			painter.drawPolyline(screenPoly);
			painter.drawLine(screenPoly.last(), screenPoly.first());
		}
	}

	for (int i = 0; i < selectedPoints->size(); i++) {
		QPointF selectPoint = gl->mapViewProjMatrix(sVert[(*selectedPoints)[i]]);
		painter.setPen(QPen(Qt::black, 8));
		painter.drawPoint(selectPoint);
		painter.setPen(QPen(Qt::red, 6));
		painter.drawPoint(selectPoint);
	}
}

void ChElementLibreSelect::clickPos(const QPointF& mouseOri)
{
	isPress = true;
	lastPos = mouseOri;

	polygon->clear();

	changeEditMode();

	if (editMode != ChElementEditMode::None) {
		affirmHandle();
		return;
	}

	QPointF mouse = gl->getViewProjMatrixInvert().map(gl->mapToGL(mouseOri));

	*polygon << mouse;

	for (unsigned int i = 0; i < sVert.size(); i++) {
		if (QLineF(sVert[i], mouse).length() < 0.02f / gl->viewScale.value()) {
			if (!selectedPoints->contains(i)) {
				if (!KeyboardStateWin::isCtrlHeld()) {
					selectedPoints->clear();
				}
				selectedPoints->append(i);
			}
			return;
		}
	}

	if (!KeyboardStateWin::isCtrlHeld()) selectedPoints->clear();
}

void ChElementLibreSelect::movePos(const QPointF& mouse)
{
	drawing = false;
	if (editMode == ChElementEditMode::None) {
		if (!isPress) return;

		QPointF mapedMouse = gl->getViewProjMatrixInvert().map(gl->mapToGL(mouse));
		if (polygon->isEmpty() || polygon->last() != mapedMouse) {
			*polygon << mapedMouse;
		}
		drawing = true;
	}
	else moveHandle(mouse);
}

void ChElementLibreSelect::releasePos(const QPointF& mouse)
{
	if (drawing) {
		drawing = false;

		if (!polygon->isEmpty()) {
			*polygon << polygon->first();
		}

		addEnclosedPoints(polygon, sVert);
	}
}

void ChElementLibreSelect::addEnclosedPoints(const QPolygonF* const poly, const QList<QPointF>& points)
{
	for (unsigned int i = 0; i < points.size(); i++) {
		if (poly->containsPoint(points[i], Qt::OddEvenFill)) {
			selectedPoints->append(i);
		}
	}
}
