#pragma once

#include <qpointf>
#include <memory>

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
	ChElementSelect(GlVertReference& glReference);
	~ChElementSelect();

	void escape();
	void enter();
	void deleteElement();
	void draw_handle_and_selected(QPainter& painter);
	void changeEditMode();
	void moveHandle(const QPointF& mouse);
	void affirmHandle();
	void click_select(const QPointF& mouse);

	GlVertReference& glVertReference;
	std::unique_ptr<SelectedPoints> selected_points;
	QPointF lastPos;
	QPointF handleCenterPoint;
	QPointF dHandleCenterPoint;
	QPointF lastHandleCenterPoint;
	QPointF lastDHandleCenterPoint;
	bool isPress = false;
	bool edit_skelen{ false };
	ChElementEditMode editMode = ChElementEditMode::None;
};