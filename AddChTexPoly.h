#pragma once

#include <QList>
#include <QPoint>
#include <memory>

#include "ChElementToolBehavior.h"

class PiYingGL;
struct GlVertReference;

struct AddChTexPoly
{
	AddChTexPoly(GlVertReference* glReference);

	void click(const QPointF& mouse);
	void enter();

	bool checkPointRepeat(const QPointF& point);

	QList<QPointF> points;
	QList<int> index;

	GlVertReference* glVertReference;
};

////////////////////////////////////////////////////////

class AddPolyEscape : public EscapeBehavior
{
public:
	AddPolyEscape(std::shared_ptr<AddChTexPoly> add) { addChTexPoly = add; }

	virtual void escape() override;

private:
	std::shared_ptr<AddChTexPoly> addChTexPoly;
};

////////////////////////////////////////////////////////

class AddPolyDelete : public DeleteElementBehavior
{
public:
	AddPolyDelete(std::shared_ptr<AddChTexPoly> add) { addChTexPoly = add; }


	virtual void deleteElement() override;

private:
	std::shared_ptr<AddChTexPoly> addChTexPoly;
};

////////////////////////////////////////////////////////

class AddPolyClick : public ClickBehavior
{
public:
	AddPolyClick(std::shared_ptr<AddChTexPoly> add) { addChTexPoly = add; }
	virtual void click(const QPointF& mouse) override;

private:
	std::shared_ptr<AddChTexPoly> addChTexPoly;
};

////////////////////////////////////////////////////////

class AddPolyDraw : public DrawBehavior
{
public:
	AddPolyDraw(std::shared_ptr<AddChTexPoly> add) { addChTexPoly = add; }
	virtual void draw(QPainter* painter) override;

private:
	std::shared_ptr<AddChTexPoly> addChTexPoly;
};

class AddPolyEnter : public EnterBehavior
{
public:
	AddPolyEnter(std::shared_ptr<AddChTexPoly> add) { addChTexPoly = add; }
	virtual void enter() override;

private:
	std::shared_ptr<AddChTexPoly> addChTexPoly;
};

