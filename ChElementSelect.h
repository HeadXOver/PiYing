#pragma once

#include "ChElementTool.h"

class SelectedPoints;

class ChElementSelect : public ChElementTool
{
public:
	ChElementSelect(int current, PiYingGL* gl);
	virtual ~ChElementSelect();

public:
	virtual void escape() override;
	virtual void enter() override {}
	virtual void deleteElement() override;

protected:
	void drawHandle(QPainter& painter);
	void changeEditMode();
	void moveHandle(const QPointF& mouse);
	void affirmHandle();

protected:
	SelectedPoints* selectedPoints;
	QPointF lastPos;
	QPointF handleCenterPoint;
	QPointF dHandleCenterPoint;
	QPointF lastHandleCenterPoint;
	QPointF lastDHandleCenterPoint;
	bool isPress = false;
	ChElementEditMode editMode = ChElementEditMode::None;
};

class ChElementRectSelect : public ChElementSelect
{
public:
	ChElementRectSelect(int current, PiYingGL* gl) :ChElementSelect(current, gl) {}

protected:
	virtual void draw(QPainter& painter) override;
	virtual void clickPos(const QPointF& mouse) override;
	virtual void movePos(const QPointF& mouse) override;
	virtual void releasePos(const QPointF& mouse) override;

private:
	QPointF rect;
	bool isDraw = false;
};