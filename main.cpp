#include "PiYing.h"

#include <QtWidgets/QApplication>
#include <qfile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile qss(":/PiYing/darkStyle.qss");
    if (qss.open(QFile::ReadOnly)) {
        app.setStyleSheet(qss.readAll());
        qss.close();
    }
    app.setFont(QFont("Segoe UI", 9));

    PiYing window;
    window.showMaximized();
    return app.exec();
}
