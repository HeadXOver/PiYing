#pragma once

#include "ChElementToolBehavior.h"

#include <memory>

struct GlVertReference;
class PiYingGL;

struct AddTriangle 
{
	AddTriangle(GlVertReference* glVertReference);

	bool addIndex(unsigned int i);
	bool checkPointRepeat(const QPointF& point);
	void addVert(const QPointF& point);
	void reduceOne();
	void click(const QPointF& mouse);
	void draw(QPainter* painter);
	void addChVert(const QPointF& point);

	GlVertReference* glVertReference;

	QPointF first;
	QPointF second;

	unsigned int firstIndex = 0;
	unsigned int secondIndex = 0;

	bool vertThenInd = false;

	int numVert = 0;
	int numInd = 0;
};

////////////////////////////////////////////////////////

class AddTriangleEscape : public EscapeBehavior
{
public:
	AddTriangleEscape(std::shared_ptr<AddTriangle> add) { addTriangle = add; }

	virtual void escape() override;

private:
	std::shared_ptr<AddTriangle> addTriangle;
};

////////////////////////////////////////////////////////

class AddTriangleDelete : public DeleteElementBehavior
{
public:
	AddTriangleDelete(std::shared_ptr<AddTriangle> add) { addTriangle = add; }


	virtual void deleteElement() override;

private:
	std::shared_ptr<AddTriangle> addTriangle;
};

////////////////////////////////////////////////////////

class AddTriangleClick : public ClickBehavior
{
public:
	AddTriangleClick(std::shared_ptr<AddTriangle> add) { addTriangle = add; }
	virtual void click(const QPointF& mouse) override;

private:
	std::shared_ptr<AddTriangle> addTriangle;
};

////////////////////////////////////////////////////////

class AddTriangleDraw : public DrawBehavior
{
public:
	AddTriangleDraw(std::shared_ptr<AddTriangle> add) { addTriangle = add; }
	virtual void draw(QPainter* painter) override;

private:
	std::shared_ptr<AddTriangle> addTriangle;
};