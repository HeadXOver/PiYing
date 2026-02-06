#pragma once

#include <QList>
#include <QPoint>
#include <memory>

#include "ch_element_tool_behavior.h"

/// <summary>
/// @brief 添加多边形
/// @details 点击多次后回车
/// </summary>
struct AddChTexPoly
{
	void click(const QPointF& mouse);
	void enter();

	bool checkPointRepeat(const QPointF& point);

	QList<QPointF> points;
	QList<int> index;
};

////////////////////////////////////////////////////////

class AddPolyEscape final : public EscapeBehavior
{
public:
	AddPolyEscape(const std::shared_ptr<AddChTexPoly>& add) : addChTexPoly(add) {}

	virtual void escape() override;

private:
	const std::shared_ptr<AddChTexPoly> addChTexPoly;
};

////////////////////////////////////////////////////////

class AddPolyDelete final : public DeleteElementBehavior
{
public:
	AddPolyDelete(const std::shared_ptr<AddChTexPoly>& add) : addChTexPoly(add) {}


	virtual void deleteElement() override;

private:
	const std::shared_ptr<AddChTexPoly> addChTexPoly;
};

////////////////////////////////////////////////////////

class AddPolyClick final : public ClickBehavior
{
public:
	AddPolyClick(const std::shared_ptr<AddChTexPoly>& add) : addChTexPoly(add) {}
	virtual void click(const QPointF& mouse) override;

private:
	const std::shared_ptr<AddChTexPoly> addChTexPoly;
};

////////////////////////////////////////////////////////

class AddPolyDraw final : public DrawBehavior
{
public:
	AddPolyDraw(const std::shared_ptr<AddChTexPoly>& add) : addChTexPoly(add) {}
	virtual void draw() override;

private:
	const std::shared_ptr<AddChTexPoly> addChTexPoly;
};

class AddPolyEnter final : public EnterBehavior
{
public:
	AddPolyEnter(const std::shared_ptr<AddChTexPoly>& add) : addChTexPoly(add) {}
	virtual void enter() override;

private:
	const std::shared_ptr<AddChTexPoly> addChTexPoly;
};

