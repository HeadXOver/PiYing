#include "ch_add_vert_trace.h"
#include "gl_vert_reference.h"
#include "point_vector_layer.h"
#include "piYingGL.h"

#include <qmessagebox>
#include <qpainter>

ChAddVertTrace::ChAddVertTrace(GlVertReference& glReference):glVertReference(glReference)
{
}

ChAddVertTrace::~ChAddVertTrace()
{
}

void ChAddVertTrace::click(const QPointF& mouseOri)
{
	pressed = true;
	const QPointF mouse = glVertReference.gl.GLViewProjMatrixInvert(mouseOri);
    const PointVectorLayer& pointVector = *(glVertReference.pointLayer);
    QPointF existPoint;
    for (unsigned int i = 0; i < pointVector.size(); i++) {
        existPoint = pointVector[i];
        if (QLineF(existPoint, mouse).length() < 0.02f / glVertReference.gl.viewScale.value()) {
            current_index = i;

            return;
        }
    }

    current_index = -1;
}

void ChAddVertTrace::move(const QPointF& mouse) 
{

}

void ChAddVertTrace::release(const QPointF& mouse) 
{
	current_index = -1;
	pressed = false;
}

void ChAddVertTrace::draw(QPainter& painter)
{
	if(current_index < 0) return;

	const PointVectorLayer& pointLayer = *(glVertReference.pointLayer);

	QPointF selectPoint = glVertReference.gl.mapViewProjMatrix(pointLayer[current_index]);
	painter.setPen(QPen(Qt::black, 8));
	painter.drawPoint(selectPoint);
	painter.setPen(QPen(Qt::red, 6));
	painter.drawPoint(selectPoint);
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

void AddVertTraceDraw::draw(QPainter* painter)
{
	addTrace->draw(*painter);
}
