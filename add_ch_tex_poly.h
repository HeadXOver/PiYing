#pragma once

#include <QList>
#include <QPoint>
#include <memory>

#include "ch_element_tool_behavior.h"

class PiYingGL;
struct GlVertReference;

struct AddChTexPoly
{
	AddChTexPoly(GlVertReference& glReference);

	void click(const QPointF& mouse);
	void enter();

	bool checkPointRepeat(const QPointF& point);

	QList<QPointF> points;
	QList<int> index;

	GlVertReference& glVertReference;
};

////////////////////////////////////////////////////////

class AddPolyEscape final : public EscapeBehavior
{
public:
	AddPolyEscape(std::shared_ptr<AddChTexPoly> add) { addChTexPoly = add; }

	virtual void escape() override;

private:
	std::shared_ptr<AddChTexPoly> addChTexPoly;
};

////////////////////////////////////////////////////////

class AddPolyDelete final : public DeleteElementBehavior
{
public:
	AddPolyDelete(std::shared_ptr<AddChTexPoly> add) { addChTexPoly = add; }


	virtual void deleteElement() override;

private:
	std::shared_ptr<AddChTexPoly> addChTexPoly;
};

////////////////////////////////////////////////////////

class AddPolyClick final : public ClickBehavior
{
public:
	AddPolyClick(std::shared_ptr<AddChTexPoly> add) { addChTexPoly = add; }
	virtual void click(const QPointF& mouse) override;

private:
	std::shared_ptr<AddChTexPoly> addChTexPoly;
};

////////////////////////////////////////////////////////

class AddPolyDraw final : public DrawBehavior
{
public:
	AddPolyDraw(std::shared_ptr<AddChTexPoly> add) { addChTexPoly = add; }
	virtual void draw(QPainter& painter) override;

private:
	std::shared_ptr<AddChTexPoly> addChTexPoly;
};

class AddPolyEnter final : public EnterBehavior
{
public:
	AddPolyEnter(std::shared_ptr<AddChTexPoly> add) { addChTexPoly = add; }
	virtual void enter() override;

private:
	std::shared_ptr<AddChTexPoly> addChTexPoly;
};

