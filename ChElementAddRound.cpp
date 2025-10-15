#include "ChElementAddRound.h"

#include <qpointf>
#include <qpainter>

void ChElementAddRound::click(const QPointF& mouse) 
{
	*center = mouse;
	isPress = true;
	radius = 0;
}

void ChElementAddRound::move(const QPointF& mouse) 
{
	radius = QLineF(*center, mouse).length();
}

void ChElementAddRound::release(const QPointF& mouse) 
{
	isPress = false;

	if (radius < 6) return;
}

void ChElementAddRound::draw(QPainter& painter)
{
	if(!isPress) return;

	if(radius < 6) return;

	painter.setPen(QPen(Qt::black, 3));
	painter.drawEllipse(*center, radius, radius);
	painter.setPen(QPen(Qt::yellow, 1));
	painter.drawEllipse(*center, radius, radius);
}

ChElementAddRound::ChElementAddRound(GlVertReference* glReference) :glVertReference(glReference)
{
	center = new QPointF(0, 0);
}

ChElementAddRound::~ChElementAddRound()
{
	delete center;
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

void AddRoundDraw::draw(QPainter* painter)
{
	addRound->draw(*painter);
}
