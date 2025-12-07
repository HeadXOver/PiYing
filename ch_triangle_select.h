#pragma once

#include "enum_select_handle_mode.h"

#include <qpointf>
#include <memory>

class SelectedPoints;
class PiYingGL;
class QPainter;
struct GlVertReference;

struct ChTriangleSelect final
{
	ChTriangleSelect(GlVertReference& glReference);
	~ChTriangleSelect();

	void escape();
	void enter();
	void deleteElement();
	void draw_handle_and_selected(QPainter& painter);
	void changeEditMode();
	void moveHandle(const QPointF& mouse);
	void affirmHandle();
	void click_select(const QPointF& mouse);
	void update_selected_to_draw();

	GlVertReference& glVertReference;
	std::unique_ptr<SelectedPoints> selected_points;

	QPointF lastPos;
	QPointF handleCenterPoint;
	QPointF dHandleCenterPoint;
	QPointF lastHandleCenterPoint;
	QPointF lastDHandleCenterPoint;

	bool edit_skelen{ false };
	ChElementEditMode editMode = ChElementEditMode::None;
};