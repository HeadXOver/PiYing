#include "ch_element_libre_select.h"

#include "gl_vert_reference.h"
#include "ch_element_select.h"
#include "piYingGL.h"
#include "SelectedPoints.h"
#include "KeyboardStateWin.h"
#include "point_vector_layer.h"

#include <qpolygonf>
#include <qpainter>
#include <qpointf>

ChElementLibreSelect::ChElementLibreSelect(GlVertReference& glReference) :edit_skelen(glReference.gl.editMode == EditMode::characterSkeleton)
{
}

void ChElementLibreSelect::draw(QPainter& painter)
{
	chElementSelect->draw_handle_and_selected(painter);

	if (!polygon.isEmpty()) {
		if (drawing) {
			painter.setPen(QPen(Qt::yellow, 1));

			auto mapper = [this](const QPointF& p) { return chElementSelect->glVertReference.gl.mapViewProjMatrix(p); };

			QPolygonF screenPoly;
			screenPoly.reserve(polygon.size());
			std::transform(polygon.cbegin(), polygon.cend(),
				std::back_inserter(screenPoly),
				mapper
			);

			painter.drawPolyline(screenPoly);
			painter.drawLine(screenPoly.last(), screenPoly.first());
		}
	}
}

void ChElementLibreSelect::clickPos(const QPointF& mouseOri)
{
	chElementSelect->isPress = true;
	chElementSelect->lastPos = mouseOri;

	polygon.clear();

	chElementSelect->changeEditMode();

	if (chElementSelect->editMode != ChElementEditMode::None) {
		chElementSelect->affirmHandle();
		return;
	}

	const QPointF mouse = chElementSelect->glVertReference.gl.getViewProjMatrixInvert().map(chElementSelect->glVertReference.gl.mapToGL(mouseOri));

	polygon << mouse;

	chElementSelect->click_select(mouse);
}

void ChElementLibreSelect::movePos(const QPointF& mouse)
{
	drawing = false;

	if (chElementSelect->editMode != ChElementEditMode::None) {
		chElementSelect->moveHandle(mouse);
		return;
	}

	if (!chElementSelect->isPress) return;

	drawing = true;

	QPointF mapedMouse = chElementSelect->glVertReference.gl.GLViewProjMatrixInvert(mouse);
	if (!polygon.isEmpty() && polygon.last() == mapedMouse) return;

	polygon << mapedMouse;
}

void ChElementLibreSelect::releasePos(const QPointF& mouse)
{
	if (!drawing) return;

	drawing = false;

	if (polygon.isEmpty()) return;

	polygon << polygon.first();

	const PointVectorLayer& points = *chElementSelect->glVertReference.pointLayer;
	QPointF existingPoint;
	for (unsigned int i = 0; i < points.size(); i++) {
		existingPoint = edit_skelen ?
			points[i] :
			points(i);
		if (polygon.containsPoint(existingPoint, Qt::OddEvenFill)) {
			chElementSelect->selected_points->append(i);
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

void LibreSelectDraw::draw(QPainter& painter)
{
	libreSelect->draw(painter);
}
