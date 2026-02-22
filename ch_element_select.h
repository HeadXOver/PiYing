#pragma once

#include <qpointf>
#include <memory>

class SelectedPoints;
enum class ChElementEditMode;

class ChElementSelect final
{
public:
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

	QPointF get_last_pos() const noexcept { return lastPos; }

	ChElementEditMode getEditMode() const noexcept { return editMode; }

	std::unique_ptr<SelectedPoints> selected_points;

	QPointF lastPos;

private:
	QPointF handleCenterPoint;
	QPointF dHandleCenterPoint;
	QPointF lastHandleCenterPoint;
	QPointF lastDHandleCenterPoint;
	
	bool edit_skelen{ false };
	ChElementEditMode editMode;
};