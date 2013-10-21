#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdbool.h>
#include "scrot.h"

bool display_init(void);
int display_num_screens(void);
void display_open_screen(int num);
struct Area *display_select_window(void);
struct Area *display_select_area(void);

#endif
