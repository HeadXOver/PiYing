#include "ch_element_add_round.h"
#include "gl_vert_reference.h"

#include "ask_round_poly_dialog.h"
#include "PiYingGL.h"

#include <qpointf>
#include <qpainter>
#include "ch_add_vert_trace.h"

namespace {
	constexpr double angle_rad = 3.1415926 / 180.0;
}

void ChElementAddRound::click(const QPointF& mouse) 
{
	center = mouse;
	current_cursor = mouse;
	isPress = true;
	radius = 0;
}

void ChElementAddRound::move(const QPointF& mouse) 
{
	radius = QLineF(center, mouse).length();
	current_cursor = mouse;
}

void ChElementAddRound::release(const QPointF& mouse) 
{
	isPress = false;

	if (radius < 6) return;

	init_angle = QLineF(center, mouse).angle();

	int init[3] {radius, 7, init_angle };
	int outRadius, outEdgeCount, outAngle;
	if (AskRoundPolyDialog(QString("设置多边形"), init, &glVertReference.gl).getValues(outRadius, outEdgeCount, outAngle)) {
		radius = outRadius;
		init_angle = outAngle;
		addRoundPoly(outEdgeCount);
	}
}

void ChElementAddRound::draw(QPainter& painter)
{
	if(!isPress) return;

	if(radius < 6) return;

	painter.setPen(QPen(Qt::black, 3));
	painter.drawEllipse(center, radius, radius);
	painter.drawLine(center, current_cursor);
	painter.setPen(QPen(Qt::yellow, 1));
	painter.drawEllipse(center, radius, radius);
	painter.drawLine(center, current_cursor);
}

void ChElementAddRound::addRoundPoly(const int edgeCount)
{
	if (edgeCount < 3) return;

	const QPointF glCenter = glVertReference.gl.GLViewProjMatrixInvert(center);
	const QPointF glCursor = glVertReference.gl.GLViewProjMatrixInvert(current_cursor);
	const float lenth = QLineF(QPointF(), glVertReference.gl.getInsProj().map(glCursor - glCenter)).length();
	const double initAngle = init_angle * angle_rad;
	const double deltaAngle = 2 * 3.1415926 / edgeCount;
	const int currentEnd = glVertReference.get_current_end();

	glVertReference.add_point_to_vert(glCenter);
	for (int i = 0; i < edgeCount; i++) {
		glVertReference.add_point_to_vert(glCenter + lenth * glVertReference.gl.getProj().map(QPointF(cos(initAngle + i * deltaAngle), sin(initAngle + i * deltaAngle))));
	}

	for (int i = 0; i < edgeCount - 1; i++) {
		glVertReference.addTriangle(currentEnd, currentEnd + i + 1, currentEnd + i + 2);
	}

	glVertReference.addTriangle(currentEnd, currentEnd + edgeCount, currentEnd + 1);
}

ChElementAddRound::ChElementAddRound(GlVertReference& glReference) :glVertReference(glReference)
{
}

ChElementAddRound::~ChElementAddRound()
{
}

void AddRoundClick::click(const QPointF& mouse)
{
	addRound->click(mouse);
}

void AddRoundMove::mouseMove(const QPointF& mouse)
{
	addRound->move(mouse);
}

void AddRoundRelease::release(const QPointF& mouse)
{
	addRound->release(mouse);
}

void AddRoundDraw::draw(QPainter& painter)
{
	addRound->draw(painter);
}
