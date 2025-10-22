#include "add_triangle.h"

#include "piYingGL.h"
#include "gl_vert_reference.h"
#include "point_vector_layer.h"

#include <qpainter>
#include <qpointf>
#include <qpen>

AddTriangle::AddTriangle(GlVertReference& glReference) :glVertReference(glReference)
{
	first = std::make_unique<QPointF>();
	second = std::make_unique<QPointF>();
}

bool AddTriangle::addVert(unsigned int i)
{
	if(numInd == 0) firstIndex = i;
	else if (numInd == 1) {
		if (firstIndex == i) return false;
		secondIndex = i;
	}
	numInd++;
	vertThenInd = true;
	return true;
}

bool AddTriangle::checkPointRepeat(const QPointF& point)
{
	if (numVert == 0) return false;
	if (numVert == 1) return *first == point;
	return *first == point || *second == point;
}

void AddTriangle::addVert(const QPointF& point)
{
	if (numVert == 0) *first = point;
	else if (numVert == 1) *second = point;
	else return;
	vertThenInd = false;
	numVert++;
}

void AddTriangle::reduceOne()
{
	if (numVert == 0 && numInd == 0) return;
	if (numVert == 0 && numInd == 1 || numVert == 1 && numInd == 0) {
		numInd = 0;
		numVert = 0;
		return;
	}
	if (numInd == 2) numInd--;
	else if (numVert == 2) numVert--;
	else if (vertThenInd) numInd--;
	else numVert--;
}

void AddTriangleDelete::deleteElement()
{
	addTriangle->numInd = 0;
	addTriangle->numVert = 0;
}

void AddTriangleClick::click(const QPointF& mouse) {
	addTriangle->click(mouse);
}

void AddTriangle::click(const QPointF& mouseOri)
{
	QPointF mouse = glVertReference.gl.getViewProjMatrixInvert().map(glVertReference.gl.mapToGL(mouseOri));

	if (checkPointRepeat(mouse))  return;

	int indRepeat = -1;
	PointVectorLayer& pointVector = *(glVertReference.pointLayer);
	for (unsigned int i = 0; i < pointVector.size(); i++) {
		if (QLineF(pointVector(i), mouse).length() < 0.02f / glVertReference.gl.viewScale.value()) {
			indRepeat = i;
			break;
		}
	}

	if (numInd + numVert <= 1) {
		if (indRepeat < 0) addVert(mouse);
		else addVert(indRepeat);
		return;
	}
	if (numVert == 2) {
		numInd = 0;
		numVert = 0;
		glVertReference.addTriangle(*first, *second, indRepeat >= 0 ? pointVector(indRepeat) : mouse);
		return;
	}
	if (numInd == 2) {
		if (indRepeat < 0) {
			numInd = 0;
			numVert = 0;
			glVertReference.addTriangle(firstIndex, secondIndex, mouse);
			return;
		}

		if (indRepeat == firstIndex || indRepeat == secondIndex) return;

		unsigned int y[3] = { (unsigned int)indRepeat, firstIndex, secondIndex };
		std::sort(y, y + 3);

		for (int j = 0; j < glVertReference.glIndex.size(); j += 3) {
			unsigned int x[3] = { glVertReference.glIndex[j + 0], glVertReference.glIndex[j + 1], glVertReference.glIndex[j + 2] };
			std::sort(x, x + 3);
			if (x[0] == y[0] && x[1] == y[1] && x[2] == y[2]) return;
		}

		numInd = 0;
		numVert = 0;
		glVertReference.addTriangle(indRepeat, firstIndex, secondIndex);
		return;
	}
	if (numInd == 1 && numVert == 1) {
		if (indRepeat < 0) {
			numInd = 0;
			numVert = 0;
			glVertReference.addTriangle(firstIndex, *first, mouse);
			return;
		}

		if (firstIndex == indRepeat) return;

		numInd = 0;
		numVert = 0;
		glVertReference.addTriangle(indRepeat, firstIndex, *first);
	}
}

void AddTriangleDraw::draw(QPainter& painter) {
	addTriangle->draw(painter);
}

void AddTriangle::draw(QPainter& painter)
{
	if (numInd == 0 && numVert == 0) return;

	std::vector<QPointF> toDraw;
	PointVectorLayer& pointVector = *(glVertReference.pointLayer);
	if (numInd == 1 && numVert == 0) {
		toDraw.push_back(pointVector(firstIndex));
	}
	else if (numInd == 0 && numVert == 1) toDraw.push_back(*first);
	else if (numVert == 2) {
		toDraw.push_back(*first);
		toDraw.push_back(*second);
	}
	else if (numInd == 2) {
		toDraw.push_back(pointVector(firstIndex));
		toDraw.push_back(pointVector(secondIndex));
	}
	else if (numInd == 1 && numVert == 1) {
		toDraw.push_back(*first);
		toDraw.push_back(pointVector(firstIndex));
	}

	for (QPointF& p : toDraw) {
		p = glVertReference.gl.mapViewProjMatrix(p);
		painter.setPen(QPen(Qt::black, 8));
		painter.drawPoint(p);
		painter.setPen(QPen(Qt::red, 6));
		painter.drawPoint(p);
	}
}

void AddTriangleEscape::escape()
{
	addTriangle->reduceOne();
}
