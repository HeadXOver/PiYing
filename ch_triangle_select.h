#pragma once

#include "enum_select_handle_mode.h"

#include <qpointf>
#include <memory>

class SelectedTriangle;

struct ChTriangleSelect final
{
	ChTriangleSelect();
	~ChTriangleSelect();

	void escape();
	void enter();
	void deleteElement();
	void draw_handle_and_selected();
	void changeEditMode();
	void moveHandle(const QPointF& mouse);
	void affirmHandle();
	void click_select(const QPointF& mouse);
	void update_selected_to_draw();

	std::unique_ptr<SelectedTriangle> selected_trangle;

	QPointF lastPos;
	QPointF handleCenterPoint;
	QPointF dHandleCenterPoint;
	QPointF lastHandleCenterPoint;
	QPointF lastDHandleCenterPoint;

	bool edit_skelen{ false };
	ChElementEditMode editMode = ChElementEditMode::None;
};