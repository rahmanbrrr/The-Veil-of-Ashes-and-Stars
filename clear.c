// utils.c
#include <stdlib.h>
#include "clear.h"

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
