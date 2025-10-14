#include "ChElementLibreSelect.h"
#include "ChElementSelect.h"
#include "piYingGL.h"
#include "SelectedPoints.h"
#include "KeyboardStateWin.h"

#include <qpolygonf>
#include <qpainter>

ChElementLibreSelect::ChElementLibreSelect(int current, PiYingGL* gl) :chElementSelect(new ChElementSelect(current, gl))
{
	polygon = new QPolygonF();
}

ChElementLibreSelect::~ChElementLibreSelect()
{
	delete polygon;
}

void ChElementLibreSelect::draw(QPainter* painter)
{
	chElementSelect->drawHandle(painter);

	if (!polygon->isEmpty()) {
		if (drawing) {
			painter->setPen(QPen(Qt::yellow, 1));

			auto mapper = [this](const QPointF& p) { return chElementSelect->glVertReference->gl->mapViewProjMatrix(p); };

			QPolygonF screenPoly;
			screenPoly.reserve(polygon->size());
			std::transform(polygon->cbegin(), polygon->cend(),
				std::back_inserter(screenPoly),
				mapper
			);

			painter->drawPolyline(screenPoly);
			painter->drawLine(screenPoly.last(), screenPoly.first());
		}
	}

	for (int i = 0; i < chElementSelect->selectedPoints->size(); i++) {
		QPointF selectPoint = chElementSelect->glVertReference->gl->mapViewProjMatrix(chElementSelect->glVertReference->sVert[(*chElementSelect->selectedPoints)[i]]);
		painter->setPen(QPen(Qt::black, 8));
		painter->drawPoint(selectPoint);
		painter->setPen(QPen(Qt::red, 6));
		painter->drawPoint(selectPoint);
	}
}

void ChElementLibreSelect::clickPos(const QPointF& mouseOri)
{
	chElementSelect->isPress = true;
	chElementSelect->lastPos = mouseOri;

	polygon->clear();

	chElementSelect->changeEditMode();

	if (chElementSelect->editMode != ChElementEditMode::None) {
		chElementSelect->affirmHandle();
		return;
	}

	QPointF mouse = chElementSelect->glVertReference->gl->getViewProjMatrixInvert().map(chElementSelect->glVertReference->gl->mapToGL(mouseOri));

	*polygon << mouse;

	for (unsigned int i = 0; i < chElementSelect->glVertReference->sVert.size(); i++) {
		if (QLineF(chElementSelect->glVertReference->sVert[i], mouse).length() < 0.02f / chElementSelect->glVertReference->gl->viewScale.value()) {
			if (!chElementSelect->selectedPoints->contains(i)) {
				if (!KeyboardStateWin::isCtrlHeld()) {
					chElementSelect->selectedPoints->clear();
				}
				chElementSelect->selectedPoints->append(i);
			}
			return;
		}
	}

	if (!KeyboardStateWin::isCtrlHeld()) chElementSelect->selectedPoints->clear();
}

void ChElementLibreSelect::movePos(const QPointF& mouse)
{
	drawing = false;
	if (chElementSelect->editMode == ChElementEditMode::None) {
		if (!chElementSelect->isPress) return;

		QPointF mapedMouse = chElementSelect->glVertReference->gl->getViewProjMatrixInvert().map(chElementSelect->glVertReference->gl->mapToGL(mouse));
		if (polygon->isEmpty() || polygon->last() != mapedMouse) {
			*polygon << mapedMouse;
		}
		drawing = true;
	}
	else chElementSelect->moveHandle(mouse);
}

void ChElementLibreSelect::releasePos(const QPointF& mouse)
{
	if (drawing) {
		drawing = false;

		if (!polygon->isEmpty()) {
			*polygon << polygon->first();
		}

		addEnclosedPoints(polygon, chElementSelect->glVertReference->sVert);
	}
}

void ChElementLibreSelect::addEnclosedPoints(const QPolygonF* const poly, const QList<QPointF>& points)
{
	for (unsigned int i = 0; i < points.size(); i++) {
		if (poly->containsPoint(points[i], Qt::OddEvenFill)) {
			chElementSelect->selectedPoints->append(i);
		}
	}
}

void LibreSelectClick::click(const QPointF& point)
{
	libreSelect->clickPos(point);
}

void LibreSelectMove::mouseMove(const QPointF& point)
{
	libreSelect->movePos(point);
}

void LibreSelectRelease::release(const QPointF& point)
{
	libreSelect->releasePos(point);
}

void LibreSelectDelete::deleteElement()
{
	libreSelect->chElementSelect->deleteElement();
}

void LibreSelectEscape::escape()
{
	libreSelect->chElementSelect->escape();
}

void LibreSelectDraw::draw(QPainter* painter)
{
	libreSelect->draw(painter);
}
