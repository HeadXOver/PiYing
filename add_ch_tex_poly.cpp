#include "add_ch_tex_poly.h"

#include "gl_vert_reference.h"
#include "piYingGL.h"
#include "point_vector_layer.h"

#include <qpainter>

AddChTexPoly::AddChTexPoly(GlVertReference& glReference) :glVertReference(glReference)
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
			index[i] = glVertReference.get_current_end();
			glVertReference.add_point_to_vert(points[i]);
		}
	}

	points.clear();

	int n1 = 0;
	int n2 = 1;
	int n3 = size - 1;
	for (int i = 2; i < size; i++) {
		glVertReference.addTriangle(index[n1], index[n2], index[n3]);
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
	QPointF mouse = glVertReference.gl.getViewProjMatrixInvert().map(glVertReference.gl.mapToGL(mouseOri));

	if (checkPointRepeat(mouse))  return;

	for (unsigned int i = 0; i < glVertReference.pointLayer->size(); i++) {
		PointVectorLayer& pointVector = *(glVertReference.pointLayer);
		const QPointF& readyPoint = pointVector.get_uv_point(i);
		if (QLineF(readyPoint, mouse).length() < 0.02f / glVertReference.gl.viewScale.value()) {
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

void AddPolyDraw::draw(QPainter* painter)
{
	for (QPointF selectPoint : addChTexPoly->points) {
		selectPoint = addChTexPoly->glVertReference.gl.mapViewProjMatrix(selectPoint);
		painter->setPen(QPen(Qt::black, 8));
		painter->drawPoint(selectPoint);
		painter->setPen(QPen(Qt::red, 6));
		painter->drawPoint(selectPoint);
	}
}

bool AddChTexPoly::checkPointRepeat(const QPointF& point)
{
	for (QPointF& readyPoint : points) {
		if (readyPoint == point) return true;
	}
	return false;
}