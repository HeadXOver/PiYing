#include "piYingGL.h"
#include "PiYing.h"
#include "ctrlSlideWidget.h"

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
    if (act) {
        int index = tempMenu.actions().indexOf(act);
        QMessageBox::information(this, "Menu", QString("id: %1").arg(sliders->get_id(index)));
    }
}