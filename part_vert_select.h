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
	void change_edit_mode_by_setting_last_pos(const QPointF& mouse);
	void moveHandle(const QPointF& mouse);
	void affirmHandle();
	void click_select(const QPointF& mouse);
	void update_selected_to_draw();
	void clear();
	void append(unsigned int index);
	void push_back(unsigned int index);

	bool contains(unsigned int index) const noexcept;
	bool is_edit_skelen() const noexcept { return _edit_shape; }

	QPointF get_last_pos() const noexcept { return lastPos; }

	ChElementEditMode getEditMode() const noexcept { return editMode; }

private:
	QPointF handleCenterPoint;
	QPointF dHandleCenterPoint;
	QPointF lastHandleCenterPoint;
	QPointF lastDHandleCenterPoint;

	QPointF lastPos;

	const bool _edit_shape;
	ChElementEditMode editMode;

	std::unique_ptr<SelectedPoints> selected_points;
};