#include "tool_vert_libre_select.h"

#include "tool_vert_select.h"
#include "piYingGL.h"
#include "selected_points.h"
#include "KeyboardStateWin.h"
#include "point_vector.h"
#include "enum_select_handle_mode.h"
#include "enum_edit_mode.h"

#include <qpolygonf>
#include <qpainter>

namespace {
	auto mapper = [](const QPointF& p) { return PiYingGL::getInstance().mapViewProjMatrix(p); };
}

piying::tool::texture::VertLibreSelect::VertLibreSelect() :
	edit_skelen(PiYingGL::getInstance().editMode == EditMode::characterSkeleton),
	chElementSelect(std::make_unique<VertSelect>())
{
}

void piying::tool::texture::VertLibreSelect::draw()
{
	chElementSelect->draw_handle_and_selected();

	if (polygon.isEmpty() || !drawing) return;

	QPainter painter(&PiYingGL::getInstance());
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QColor(225, 0, 0, 20));
	painter.setPen(QPen(Qt::yellow, 1));

	QPolygonF screenPoly;
	screenPoly.reserve(polygon.size());
	std::transform(polygon.cbegin(), polygon.cend(),
		std::back_inserter(screenPoly),
		mapper
	);

	painter.drawPolyline(screenPoly);
	painter.drawLine(screenPoly.last(), screenPoly.first());
}

void piying::tool::texture::VertLibreSelect::click(const QPointF& mouseOri)
{
	isPress = true;

	polygon.clear();

	chElementSelect->change_edit_mode_by_setting_last_pos(mouseOri);

	if (chElementSelect->getEditMode() != HandleControlMode::None) {
		chElementSelect->affirmHandle();
		return;
	}

	const QPointF mouse = PiYingGL::getInstance().getViewProjMatrixInvert().map(PiYingGL::getInstance().mapToGL(mouseOri));

	polygon << mouse;

	chElementSelect->click_select(mouse);
}

void piying::tool::texture::VertLibreSelect::escape()
{
	if (isPress) return;
	chElementSelect->escape();
}

void piying::tool::texture::VertLibreSelect::mouse_move(const QPointF& mouse)
{
	drawing = false;

	if (!isPress) return;

	if (chElementSelect->getEditMode() != HandleControlMode::None) {
		chElementSelect->moveHandle(mouse);
		return;
	}

	drawing = true;

	QPointF mapedMouse = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse);
	if (!polygon.isEmpty() && polygon.last() == mapedMouse) return;

	polygon << mapedMouse;
}

void piying::tool::texture::VertLibreSelect::release(const QPointF& mouse)
{
	isPress = false;

	if (!drawing) return;

	drawing = false;

	if (polygon.isEmpty()) return;

	polygon << polygon.first();

	if (!KeyboardStateWin::isCtrlHeld()) chElementSelect->clear();

	const PointVectorLayer& points = *PiYingGL::getInstance().currentLayer();
	QPointF existingPoint;
	for (unsigned int i = 0; i < points.element_size(); i++) {
		existingPoint = points.get(i, edit_skelen);
		if (polygon.containsPoint(existingPoint, Qt::OddEvenFill) && !chElementSelect->contains(i)) {
			chElementSelect->append(i);
		}
	}

	chElementSelect->update_selected_to_draw();
}

void piying::tool::texture::VertLibreSelect::delete_element()
{
	chElementSelect->deleteElement();
}