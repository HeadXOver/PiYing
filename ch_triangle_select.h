#pragma once

#include <qpointf>
#include <memory>

class SelectedTriangle;

enum class ChElementEditMode;

class ChTriangleSelect final
{
public:
	ChTriangleSelect();
	~ChTriangleSelect();

	ChElementEditMode get_edit_mode() const noexcept { return editMode; }

	void escape();
	void enter();
	void deleteElement();
	void draw_handle_and_selected();
	void changeEditMode();
	void moveHandle(const QPointF& mouse);
	void affirmHandle();
	void click_select(const QPointF& mouse);
	void update_selected_to_draw();

public:
	std::unique_ptr<SelectedTriangle> selected_trangle;

	QPointF lastPos;

private:
	QPointF handleCenterPoint;
	QPointF dHandleCenterPoint;
	QPointF lastHandleCenterPoint;
	QPointF lastDHandleCenterPoint;

	bool edit_skelen{ false };
	ChElementEditMode editMode{ 0 };
};