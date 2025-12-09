#include "add_ch_tex_poly.h"

#include "piYingGL.h"
#include "global_objects.h"
#include "point_vector_layer.h"

#include <qpainter>

AddChTexPoly::AddChTexPoly()
{
}

AddChTexPoly::~AddChTexPoly()
{
}

void AddPolyEscape::escape()
{
	if (addChTexPoly->index.size() == 0) return;

	addChTexPoly->index.removeLast();
	addChTexPoly->points.removeLast();
}

void AddPolyEnter::enter() 
{
	addChTexPoly->enter();
}

void AddChTexPoly::enter()
{
	int size = index.size();
	if (size < 3) return;

	for (int i = 0; i < index.size(); i++) {
		if (index[i] < 0) {
			index[i] = (int)currentLayer->size();
			piYingGL->add_point_to_vert(points[i]);
		}
	}

	points.clear();

	int n1 = 0;
	int n2 = 1;
	int n3 = size - 1;
	for (int i = 2; i < size; i++) {
		piYingGL->addTriangle(index[n1], index[n2], index[n3]);
		n1 = n2;
		n2 = n3;
		n3 = i / 2 + 1;
		if (i % 2) {
			n3 = size - n3;
		}
	}

	index.clear();
}

void AddPolyDelete::deleteElement()
{
	addChTexPoly->index.clear();
	addChTexPoly->points.clear();
}

void AddPolyClick::click(const QPointF& mouse) 
{
	addChTexPoly->click(mouse);
}

void AddChTexPoly::click(const QPointF& mouseOri)
{
	QPointF mouse = piYingGL->getViewProjMatrixInvert().map(piYingGL->mapToGL(mouseOri));

	if (checkPointRepeat(mouse))  return;

	for (unsigned int i = 0; i < currentLayer->size(); i++) {
		const QPointF& readyPoint = currentLayer->get(i, false);
		if (QLineF(readyPoint, mouse).length() < 0.02f / piYingGL->viewScale.value()) {
			if (!index.contains(i)) {
				index.append(i);
				points.append(readyPoint);
			}
			return;
		}
	}

	index.append(-1);
	points.append(mouse);
}

void AddPolyDraw::draw()
{
	QPainter painter(piYingGL);
	painter.setRenderHint(QPainter::Antialiasing);

	for (QPointF selectPoint : addChTexPoly->points) {
		selectPoint = piYingGL->mapViewProjMatrix(selectPoint);
		painter.setPen(QPen(Qt::black, 8));
		painter.drawPoint(selectPoint);
		painter.setPen(QPen(Qt::red, 6));
		painter.drawPoint(selectPoint);
	}
}

bool AddChTexPoly::checkPointRepeat(const QPointF& point)
{
	for (const QPointF& readyPoint : points) {
		if (readyPoint == point) return true;
	}
	return false;
}