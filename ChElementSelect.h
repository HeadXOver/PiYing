#pragma once

#include <qpointf>

class SelectedPoints;
class PiYingGL;
class QPainter;
struct GlVertReference;

enum class ChElementEditMode {
	None,
	Move,
	MoveX,
	MoveY,
	Rotate,
	Scale,
	ScaleX,
	ScaleY,
};

struct ChElementSelect final
{
	ChElementSelect(GlVertReference* glReference);
	virtual ~ChElementSelect();

	void escape();
	void deleteElement();
	void drawHandle(QPainter* painter);
	void changeEditMode();
	void moveHandle(const QPointF& mouse);
	void affirmHandle();

	GlVertReference* glVertReference;
	SelectedPoints* selectedPoints;
	QPointF lastPos;
	QPointF handleCenterPoint;
	QPointF dHandleCenterPoint;
	QPointF lastHandleCenterPoint;
	QPointF lastDHandleCenterPoint;
	bool isPress = false;
	ChElementEditMode editMode = ChElementEditMode::None;
};