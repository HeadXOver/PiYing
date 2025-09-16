#include "PiYing.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    PiYing window;
    window.showMaximized();
    return app.exec();
}
