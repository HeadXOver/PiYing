#include "ch_element_add_round.h"

#include "ask_round_poly_dialog.h"
#include "point_vector_layer.h"
#include "PiYingGL.h"

#include <qpainter>

namespace {
	constexpr double angle_rad = 3.1415926 / 180.0;

	constexpr float DEFAULT_RATIO = 16.0f / 9.0f;

	QMatrix4x4 get_proj() {
		QMatrix4x4 proj;
		proj.ortho(-DEFAULT_RATIO, DEFAULT_RATIO, -1, 1, -1, 1);
		return proj;
	}

	QMatrix4x4 get_proj_invert() {
		QMatrix4x4 proj;
		proj.ortho(-1.0f / DEFAULT_RATIO, 1.0f / DEFAULT_RATIO, -1, 1, -1, 1);
		return proj;
	}
}

void ChElementAddRound::click(const QPointF& mouse) 
{
	center = mouse;
	gl_center = PiYingGL::getInstance().GLViewProjMatrixInvert(center);
	current_cursor = mouse;
	isPress = true;
	radius = 0;
}

void ChElementAddRound::mouse_move(const QPointF& mouse) 
{
	radius = QLineF(center, mouse).length();
	current_cursor = mouse;
}

void ChElementAddRound::release(const QPointF& mouse) 
{
	if (radius < 6) {
		isPress = false;
		return;
	}

	init_angle = QLineF(center, mouse).angle();

	const QPointF glCursor = PiYingGL::getInstance().GLViewProjMatrixInvert(current_cursor);
	const float lenth = QLineF(QPointF(), get_proj_invert().map(glCursor - gl_center)).length();

	radius = lenth * 1000.f;

	int init[3] {radius, 7, init_angle };
	int outEdgeCount;
	if (AskRoundPolyDialog(QString("设置多边形"), init, &PiYingGL::getInstance()).getValues(radius, outEdgeCount, init_angle)) {
		addRoundPoly(outEdgeCount);
	}

	isPress = false;
}

void ChElementAddRound::draw()
{
	if(!isPress) return;

	if(radius < 6) return;

	QPainter painter(&PiYingGL::getInstance());
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QColor(225, 0, 0, 20));

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

	const float lenth = radius / 1000.f;
	const double initAngle = init_angle * angle_rad;
	const double deltaAngle = (2 * 3.1415926) / edgeCount;
	const int currentEnd = (int)PiYingGL::getInstance().currentLayer()->size();

	PiYingGL::getInstance().add_point_to_vert(gl_center);
	for (int i = 0; i < edgeCount; i++) {
		PiYingGL::getInstance().add_point_to_vert(gl_center + lenth * get_proj().map(QPointF(cos(initAngle + i * deltaAngle), sin(initAngle + i * deltaAngle))));
	}

	for (int i = 0; i < edgeCount - 1; i++) {
		PiYingGL::getInstance().addTriangle(currentEnd, currentEnd + i + 1, currentEnd + i + 2);
	}

	PiYingGL::getInstance().addTriangle(currentEnd, currentEnd + edgeCount, currentEnd + 1);
}
