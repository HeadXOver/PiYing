#include "piYingGL.h"
#include "PiYing.h"
#include "ctrlSlideWidget.h"
#include "slide_applier.h"

#include <qmenu>
#include <qmessagebox>
#include <QCursor>

void PiYingGL::add_trace(int index, const QPolygonF& polygon)
{
    CtrlSlideWidget* sliders = ref_PiYing.sliderWidget[getCurrentChRow()];
    QList<QString> items = sliders->get_slider_names();

    QMenu tempMenu(this);

    tempMenu.addAction(QString("新建控制器"));

    for (int i = 0; i < items.size(); ++i)
        tempMenu.addAction(QString("绑定到: %1").arg(items[i]));

    QAction* act = tempMenu.exec(QCursor::pos());
    if (!act) return;

    int id = tempMenu.actions().indexOf(act) - 1;

    if (id == -1) {
        sliders->addSlider();
        id = items.size();
    }

    if(!sliders->add_trace(sliders->get_id(id), index, polygon)) QMessageBox::warning(this, "警告", "轨迹重复");
}
