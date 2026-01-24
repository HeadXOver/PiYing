#include "ch_add_vert_trace.h"
#include "point_vector_layer.h"
#include "piYingGL.h"
#include "time_line_gl.h"
#include "part.h"
#include "global_objects.h"

#include <qmessagebox>
#include <qpainter>

ChAddVertTrace::ChAddVertTrace()
{
}

ChAddVertTrace::~ChAddVertTrace()
{
}

void ChAddVertTrace::click(const QPointF& mouseOri)
{
	const QPointF mouse = piYingGL->GLViewProjMatrixInvert(mouseOri);

	Part* part = timelineGl->get_current_part();

    QPointF existPoint;
    for (unsigned int i = 0; i < part->vertex_size() / 2; i++) {
		existPoint = part->get_vert(i, true);
        if (QLineF(existPoint, mouse).length() < 0.02f / piYingGL->viewScale.value()) {
            current_index = i;
			presse_on_vert = true;
			polygon.clear();
			polygon << existPoint;
			polygon << mouse;

            return;
        }
    }

    current_index = -1;
}

void ChAddVertTrace::move(const QPointF& mouse) 
{
	if(!presse_on_vert) return;

	QPointF glMouse = piYingGL->GLViewProjMatrixInvert(mouse);
	if (polygon.isEmpty() || polygon.last() != glMouse) polygon << glMouse;
}

void ChAddVertTrace::release(const QPointF& mouse) 
{
	presse_on_vert = false;

	if (polygon.size() < 3) {
		current_index = -1;
		polygon.clear();
		return;
	}

	Part* part = timelineGl->get_current_part();
	if (part) part->add_trace(current_index, polygon);

	current_index = -1;
	polygon.clear();
}

void ChAddVertTrace::draw()
{
	if (current_index < 0) return;

	const PointVectorLayer& pointLayer = *currentLayer;

	QPointF selectPoint = piYingGL->mapViewProjMatrix(timelineGl->get_current_part()->get_vert(current_index, true));

	QPainter painter(piYingGL);
	painter.setPen(QPen(Qt::black, 8));
	painter.drawPoint(selectPoint);
	painter.setPen(QPen(Qt::red, 6));
	painter.drawPoint(selectPoint);

	if (polygon.isEmpty()) return;

	painter.setPen(QPen(Qt::yellow, 1));

	QPolygonF screenPoly;
	screenPoly.reserve(polygon.size());
	std::transform(polygon.cbegin(), polygon.cend(),
		std::back_inserter(screenPoly),
		[this](const QPointF& p) { return piYingGL->mapViewProjMatrix(p); }
	);

	painter.drawPolyline(screenPoly);
}

void AddVertTraceClick::click(const QPointF& mouse)
{
	addTrace->click(mouse);
}

void AddVertTraceMove::mouseMove(const QPointF& mouse)
{
	addTrace->move(mouse);
}

void AddVertTraceRelease::release(const QPointF& mouse)
{
	addTrace->release(mouse);
}

void AddVertTraceDraw::draw()
{
	addTrace->draw();
}
