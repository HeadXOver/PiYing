#include "tool_triangle_rect_select.h"

#include "tool_triangle_select.h"
#include "piYingGL.h"
#include "selected_points.h"
#include "KeyboardStateWin.h"
#include "point_vector.h"

#include "enum_select_handle_mode.h"
#include "enum_edit_mode.h"

#include "cus_func_zone.h"

#include <qpainter>
#include <qmessagebox>
#include <array>

piying::tool::texture::TriangleRectSelect::TriangleRectSelect() :
	edit_skelen(PiYingGL::getInstance().editMode == EditMode::characterSkeleton),
	chTriangleSelect(std::make_unique<TriangleSelect>())
{
}

void piying::tool::texture::TriangleRectSelect::draw()
{
	chTriangleSelect->draw_handle_and_selected();

	QPainter painter(&PiYingGL::getInstance());
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QColor(225, 0, 0, 20));

	if (isDraw) {
		painter.setPen(QPen(Qt::yellow, 1));
		painter.drawRect(chTriangleSelect->lastPos.x(), chTriangleSelect->lastPos.y(), rect.x() - chTriangleSelect->lastPos.x(), rect.y() - chTriangleSelect->lastPos.y());
	}
}

void piying::tool::texture::TriangleRectSelect::click(const QPointF& mouse)
{
	_is_press = true;
	chTriangleSelect->mouse_press(mouse);
}

void piying::tool::texture::TriangleRectSelect::mouse_move(const QPointF& mouse)
{
	if (!_is_press) return;

	if (chTriangleSelect->get_edit_mode() != ToolHandleControlMode::None) {
		chTriangleSelect->moveHandle(mouse);
		isDraw = false;
		return;
	}

	rect = mouse;
	isDraw = true;
}

void piying::tool::texture::TriangleRectSelect::release(const QPointF& mouse)
{
	_is_press = false;
	if (!isDraw) {
		const QPointF glMouse = PiYingGL::getInstance().GLViewProjMatrixInvert(mouse);

		chTriangleSelect->click_select(glMouse);
		return;
	}
	isDraw = false;

	if (!KeyboardStateWin::isCtrlHeld()) chTriangleSelect->selected_trangle->clear();

	const PointVectorLayer& pointVector = *PiYingGL::getInstance().currentLayer();
	const std::vector<unsigned int>& triangleIndices = *PiYingGL::getInstance().currentIndex();

	QRectF rect(chTriangleSelect->lastPos, mouse);
	QPointF eachTriangle[3];
	const bool easySelect = mouse.x() > chTriangleSelect->lastPos.x();
	std::array<unsigned int, 3> triangle;
	bool containTriangle;
	for (unsigned int i = 0; i < triangleIndices.size(); i += 3) {
		for (int j = 0; j < 3; ++j) eachTriangle[j] = PiYingGL::getInstance().mapViewProjMatrix(pointVector.get(triangleIndices[i + j], edit_skelen));

		triangle = {triangleIndices[i], triangleIndices[i + 1], triangleIndices[i + 2]};
		containTriangle = chTriangleSelect->selected_trangle->contains(triangle);
		if (rect.contains(eachTriangle[0]) &&
			rect.contains(eachTriangle[1]) &&
			rect.contains(eachTriangle[2]) &&
			!containTriangle
			) {
			chTriangleSelect->selected_trangle->append(triangle);
		}
		else if (easySelect && piying::isRectIntersectTriangle(rect, eachTriangle)) {
			if (!containTriangle) {
				chTriangleSelect->selected_trangle->append(triangle);
			}
		}
	}

	chTriangleSelect->update_selected_to_draw();
}

void piying::tool::texture::TriangleRectSelect::enter()
{
	chTriangleSelect->enter();
}

void piying::tool::texture::TriangleRectSelect::delete_element()
{
	chTriangleSelect->deleteElement();
}

void piying::tool::texture::TriangleRectSelect::escape()
{
	chTriangleSelect->escape();
}