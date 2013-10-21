#include "scrot.h"
#include "display.h"
#include "display_X11.h"

bool display_init(void)
{
	return display_X11_init();
}

int display_num_screens(void)
{
	return display_X11_num_screens();
}

void display_open_screen(int num)
{
	display_X11_open_screen(num);
}

struct Area *display_select_area(void)
{
	return display_X11_select_area();
}

struct Area *display_select_window(void)
{
	return display_X11_select_window();
}
