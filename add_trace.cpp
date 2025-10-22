#include "piYingGL.h"
#include "PiYing.h"
#include "ctrlSlideWidget.h"
#include "slide_applier.h"

#include <qmenu>
#include <qmessagebox>

void PiYingGL::add_trace(int index, const QPolygonF& polygon, const QPoint& mouse)
{
    CtrlSlideWidget* sliders = ref_PiYing.sliderWidget[getCurrentChRow()];
    QList<QString> items = sliders->get_slider_names();

    QMenu tempMenu(this);

    for (int i = 0; i < items.size(); ++i)
        tempMenu.addAction(QString("绑定到: %2").arg(items[i]));

    QAction* act = tempMenu.exec(mouse);
    if (!act) return;

    int id = tempMenu.actions().indexOf(act);
    if(!slide_applier->add_trace(id, index, polygon)) QMessageBox::warning(this, "警告", "轨迹重复");
}