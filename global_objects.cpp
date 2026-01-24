#include "global_objects.h"

#include "part.h"

void g_update_order()
{
    for (int i = 0; i < parts.size(); i++) {
        parts[i]->index = i;
    }
}
