#include "cus_readfile.h"

#include <qfile>

QString PiYingCus::readAllFileToQString(const QString& filePath)
{
    QString str;
    QFile qss(filePath);
    if (qss.open(QFile::ReadOnly)) {
        str = qss.readAll();
        qss.close();
    }

    return str;
}
