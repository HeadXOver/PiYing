#include "ch_add_vert_trace.h"
#include "gl_vert_reference.h"

#include <qmessagebox>

ChAddVertTrace::ChAddVertTrace(GlVertReference& glReference):glVertReference(glReference)
{
}

ChAddVertTrace::~ChAddVertTrace()
{
}

void ChAddVertTrace::click(const QPointF& mouse) {
	QMessageBox::information(nullptr, "click", "click");
}

void ChAddVertTrace::move(const QPointF& mouse) {

}

void ChAddVertTrace::release(const QPointF& mouse) {

}

void ChAddVertTrace::draw(QPainter& painter) {

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
