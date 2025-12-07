#pragma once

#include <qpointf>
#include <memory>

class SelectedPoints;
class PiYingGL;
class QPainter;
struct GlVertReference;
enum class ChElementEditMode;

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
	void uodate_selected_to_draw();

	GlVertReference& glVertReference;
	std::unique_ptr<SelectedPoints> selected_points;

	QPointF lastPos;
	QPointF handleCenterPoint;
	QPointF dHandleCenterPoint;
	QPointF lastHandleCenterPoint;
	QPointF lastDHandleCenterPoint;
	
	bool edit_skelen{ false };
	ChElementEditMode editMode;
};