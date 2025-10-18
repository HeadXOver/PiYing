#include "cus_func_get_unique_id.h"

#include "ctrlSlideLayout.h"

int get_unique_id(QList<CtrlSlideLayout*> sliderList)
{
    bool repeat = false;
    for (int i = 1;; i++) {
        repeat = false;
        for (int j = 0; j < sliderList.size(); ++j)
            if (sliderList[j]->id_ == i) {
                repeat = true;
                break;
            }
        if (!repeat) return i;
    }

    return 0;
}
