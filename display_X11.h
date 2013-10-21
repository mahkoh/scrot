#ifndef _DISPLAY_X11_H_
#define _DISPLAY_X11_H_

#include <X11/Xlib.h>

extern Display *disp;
extern Screen *scr;

void display_X11_init(void);
int display_X11_num_screens(void);
void display_X11_open_screen(int num);
struct Area display_X11_select_window(void);
struct Area display_X11_select_area(void);

#endif
