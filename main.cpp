#include "PiYing.h"

#include "cus_readfile.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv); app.setStyleSheet(PiYingCus::readAllFileToQString(":/PiYing/darkStyle.qss"));

    PiYing window; window.showMaximized();

    return app.exec();
}