#include "tool_add_poly.h"

#include "piYingGL.h"
#include "point_vector.h"

#include <qpainter>

void piying::tool::texture::AddPoly::escape()
{
	if (index.size() == 0) return;

	index.removeLast();
	points.removeLast();
}

void piying::tool::texture::AddPoly::enter()
{
	const int size = (int)index.size();
	if (size < 3) return;

	for (int i = 0; i < size; i++) {
		if (index[i] < 0) {
			index[i] = (int)PiYingGL::getInstance().currentLayer()->element_size();
			PiYingGL::getInstance().add_point_to_vert(points[i]);
		}
	}

	points.clear();

	int n1 = 0;
	int n2 = 1;
	int n3 = size - 1;
	for (int i = 2; i < size; i++) {
		PiYingGL::getInstance().addTriangle(index[n1], index[n2], index[n3]);
		n1 = n2;
		n2 = n3;
		n3 = i / 2 + 1;
		if (i % 2) {
			n3 = size - n3;
		}
	}

	index.clear();
}

void piying::tool::texture::AddPoly::delete_element()
{
	index.clear();
	points.clear();
}

void piying::tool::texture::AddPoly::click(const QPointF& mouseOri)
{
	PiYingGL& piYingGL = PiYingGL::getInstance();

	QPointF mouse = piYingGL.GLViewProjMatrixInvert(mouseOri);

	if (checkPointRepeat(mouse))  return;

	const PointVectorLayer& layer = *piYingGL.currentLayer();

	for (unsigned int i = 0; i < layer.element_size(); i++) {
		const QPointF& readyPoint = layer.get(i, false);
		if (QLineF(readyPoint, mouse).length() < 0.02f / piYingGL.viewScale.value()) {
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

void piying::tool::texture::AddPoly::draw()
{
	PiYingGL& piYingGL = PiYingGL::getInstance();

	QPainter painter(&piYingGL);
	painter.setRenderHint(QPainter::Antialiasing);

	for (QPointF selectPoint : points) {
		selectPoint = piYingGL.mapViewProjMatrix(selectPoint);
		painter.setPen(QPen(Qt::black, 8));
		painter.drawPoint(selectPoint);
		painter.setPen(QPen(Qt::red, 6));
		painter.drawPoint(selectPoint);
	}
}

bool piying::tool::texture::AddPoly::checkPointRepeat(const QPointF& point)
{
	for (const QPointF& readyPoint : points) {
		if (readyPoint == point) return true;
	}
	return false;
}