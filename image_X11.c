#include <X11/Xlib.h>
#include <Imlib2.h>
#include <string.h>

#include "display_X11.h"
#include "image.h"
#include "image_X11.h"

void image_X11_init(void)
{
	Visual *vis = DefaultVisual(disp, XScreenNumberOfScreen(scr));
	Colormap cm = DefaultColormap(disp, XScreenNumberOfScreen(scr));

	imlib_context_set_display(disp);
	imlib_context_set_visual(vis);
	imlib_context_set_colormap(cm);
	imlib_context_set_color_modifier(NULL);
	imlib_context_set_operation(IMLIB_OP_COPY);
}

Image image_X11_from_screen(void)
{
	imlib_context_set_drawable(scr->root);
	return imlib_create_image_from_drawable(0, 0, 0, scr->width, scr->height, 1);
}

Image image_X11_from_area(struct Area *area)
{
	imlib_context_set_drawable(scr->root);
	return imlib_create_image_from_drawable(0, area->x, area->y, area->width, area->height, 1);
}
