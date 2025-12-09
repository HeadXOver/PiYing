#include "ch_element_libre_select.h"

#include "gl_vert_reference.h"
#include "ch_element_select.h"
#include "piYingGL.h"
#include "global_objects.h"
#include "SelectedPoints.h"
#include "KeyboardStateWin.h"
#include "point_vector_layer.h"
#include "enum_select_handle_mode.h"
#include "enum_edit_mode.h"

#include <qpolygonf>
#include <qpainter>
#include <qpointf>

ChElementLibreSelect::ChElementLibreSelect(GlVertReference& glReference) :
	edit_skelen(piYingGL->editMode == EditMode::characterSkeleton)
{
	chElementSelect = std::make_unique<ChElementSelect>(glReference);
}

void ChElementLibreSelect::draw()
{
	chElementSelect->draw_handle_and_selected();

	QPainter painter(piYingGL);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QColor(225, 0, 0, 20));

	if (!polygon.isEmpty()) {
		if (drawing) {
			painter.setPen(QPen(Qt::yellow, 1));

			auto mapper = [this](const QPointF& p) { return piYingGL->mapViewProjMatrix(p); };

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
	isPress = true;
	chElementSelect->lastPos = mouseOri;

	polygon.clear();

	chElementSelect->changeEditMode();

	if (chElementSelect->editMode != ChElementEditMode::None) {
		chElementSelect->affirmHandle();
		return;
	}

	const QPointF mouse = piYingGL->getViewProjMatrixInvert().map(piYingGL->mapToGL(mouseOri));

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

	if (!isPress) return;

	drawing = true;

	QPointF mapedMouse = piYingGL->GLViewProjMatrixInvert(mouse);
	if (!polygon.isEmpty() && polygon.last() == mapedMouse) return;

	polygon << mapedMouse;
}

void ChElementLibreSelect::releasePos(const QPointF& mouse)
{
	isPress = false;

	if (!drawing) return;

	drawing = false;

	if (polygon.isEmpty()) return;

	polygon << polygon.first();

	const PointVectorLayer& points = *chElementSelect->glVertReference.pointLayer;
	QPointF existingPoint;
	for (unsigned int i = 0; i < points.size(); i++) {
		existingPoint = points.get(i, edit_skelen);
		if (polygon.containsPoint(existingPoint, Qt::OddEvenFill)) {
			chElementSelect->selected_points->append(i);
		}
	}

	chElementSelect->update_selected_to_draw();
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

void LibreSelectDraw::draw()
{
	libreSelect->draw();
}