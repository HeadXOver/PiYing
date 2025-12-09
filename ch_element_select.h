#pragma once

#include <qpointf>
#include <memory>

class SelectedPoints;
enum class ChElementEditMode;

struct ChElementSelect final
{
	ChElementSelect();
	~ChElementSelect();

	void escape();
	void deleteElement();
	void draw_handle_and_selected();
	void changeEditMode();
	void moveHandle(const QPointF& mouse);
	void affirmHandle();
	void click_select(const QPointF& mouse);
	void update_selected_to_draw();

	std::unique_ptr<SelectedPoints> selected_points;

	QPointF lastPos;
	QPointF handleCenterPoint;
	QPointF dHandleCenterPoint;
	QPointF lastHandleCenterPoint;
	QPointF lastDHandleCenterPoint;
	
	bool edit_skelen{ false };
	ChElementEditMode editMode;
};