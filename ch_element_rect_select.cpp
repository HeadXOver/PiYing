#include "ch_element_rect_select.h"

#include "ch_element_select.h"
#include "piYingGL.h"
#include "SelectedPoints.h"
#include "KeyboardStateWin.h"
#include "point_vector.h"

#include "enum_select_handle_mode.h"
#include "enum_edit_mode.h"

#include <qpainter>
#include <qpointf>
#include <qmessagebox>

ChElementRectSelect::ChElementRectSelect() :
	chElementSelect(std::make_unique<ChElementSelect>())
{
}

void ChElementRectSelect::draw()
{
	chElementSelect->draw_handle_and_selected();

	QPainter painter(&PiYingGL::getInstance());
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QColor(225, 0, 0, 20));

	if (isDraw) {
		painter.setPen(QPen(Qt::yellow, 1));
		const QPointF& lastPos = chElementSelect->get_last_pos();
		painter.drawRect(lastPos.x(), lastPos.y(), rect.x() - lastPos.x(), rect.y() - lastPos.y());
	}
}

void ChElementRectSelect::click(const QPointF& mouseOri)
{
	chElementSelect->change_edit_mode_by_setting_last_pos(mouseOri);

	if (chElementSelect->getEditMode() != ChElementEditMode::None) {
		chElementSelect->affirmHandle();
		return;
	}

	const QPointF mouse = PiYingGL::getInstance().GLViewProjMatrixInvert(mouseOri);

	chElementSelect->click_select(mouse);
}

void ChElementRectSelect::mouse_move(const QPointF& mouse)
{
	if (chElementSelect->getEditMode() != ChElementEditMode::None) {
		chElementSelect->moveHandle(mouse);
		isDraw = false;
		return;
	}

	rect = mouse;
	isDraw = true;
}

void ChElementRectSelect::release(const QPointF& mouse)
{
	if (!isDraw) return;
	isDraw = false;

	if(!KeyboardStateWin::isCtrlHeld()) chElementSelect->clear();

	const QRectF rect(chElementSelect->get_last_pos(), mouse);
	const PointVectorLayer& pointVector = *PiYingGL::getInstance().currentLayer();
	QPointF pos;
	for (unsigned int i = 0; i < pointVector.size(); i++) {
		pos = PiYingGL::getInstance().mapViewProjMatrix(pointVector.get(i, chElementSelect->is_edit_skelen()));
		if (rect.contains(pos) && !chElementSelect->contains(i)) {
			chElementSelect->append(i);
		}
	}

	chElementSelect->update_selected_to_draw();
}

void ChElementRectSelect::delete_element()
{
	chElementSelect->deleteElement();
}

void ChElementRectSelect::escape()
{
	chElementSelect->escape();
}
