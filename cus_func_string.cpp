#include "cus_func_string.h"

#include <qstring>
#include <QListWidget>

QString getUniquebgName(const QListWidget* list) {
    QString s;
    bool repeat = false;
    for (int i = 1;; i++) {
        s = QString::number(i);
        repeat = false;
        for (int j = 0; j < list->count(); ++j)
            if (list->item(j)->text() == s) repeat = true;
        if (!repeat) return s;
    }
}
