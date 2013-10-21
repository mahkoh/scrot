#include <sys/select.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>

#include "scrot.h"
#include "display.h"
#include "display_X11.h"
#include "options.h"

#ifndef min
#define min(x,y) (((x)<(y))?(x):(y))
#endif

Display *disp;
Screen *scr;
Window root;

bool display_X11_init(void)
{
	disp = XOpenDisplay(NULL);
	if (disp == NULL)
		return false;
	scr = ScreenOfDisplay(disp, DefaultScreen(disp));
	root = RootWindow(disp, XScreenNumberOfScreen(scr));
	return true;
}

int display_X11_num_screens(void)
{
	return ScreenCount(disp);
}

void display_X11_open_screen(int num)
{
	scr = ScreenOfDisplay(disp, num);
	root = RootWindow(disp, XScreenNumberOfScreen(scr));
}

static inline bool display_X11_process_events(GC gc, int start_x, int start_y, struct Area *area)
{
	XEvent ev;

	while (XPending(disp)) {
		XNextEvent(disp, &ev);
		switch (ev.type) {
		case MotionNotify:
			XDrawRectangle(disp, root, gc, area->x, area->y, area->width, area->height);

			area->x = min(start_x, ev.xmotion.x);
			area->y = min(start_y, ev.xmotion.y);
			area->width = abs(ev.xmotion.x - start_x);
			area->height = abs(ev.xmotion.y - start_y);

			XDrawRectangle(disp, root, gc, area->x, area->y, area->width, area->height);
			XFlush(disp);
			break;
		case ButtonRelease:
			if (area->width > 5 || area->height > 5) {
				XDrawRectangle(disp, root, gc, area->x, area->y, area->width, area->height);
				return false;
			}
		}
	}

	return true;
}

static void display_X11_area_sanitize(struct Area *area)
{
	if (area->x < 0) {
		area->width += area->x;
		area->x = 0;
	}
	if (area->y < 0) {
		area->height += area->y;
		area->y = 0;
	}
	if (area->x + area->width > scr->width)
		area->width = scr->width - area->x;
	if (area->y + area->height > scr->height)
		area->height = scr->height - area->y;
}

static GC display_X11_create_gc(void)
{
	XGCValues gcval;
	gcval.foreground = XWhitePixel(disp, 0);
	gcval.function = GXxor;
	gcval.background = XBlackPixel(disp, 0);
	gcval.plane_mask = gcval.background ^ gcval.foreground;
	gcval.subwindow_mode = IncludeInferiors;

	return XCreateGC(disp, root, GCFunction | GCForeground | GCBackground | GCSubwindowMode, &gcval);
}

struct Area *display_X11_select_area(void)
{
	struct Area *area = calloc(1, sizeof(*area));

	Cursor cursor = XCreateFontCursor(disp, XC_left_ptr);
	Cursor cursor2 = XCreateFontCursor(disp, XC_lr_angle);

	GC gc = display_X11_create_gc();

	if (XGrabPointer(disp, root, False, ButtonPressMask, GrabModeAsync, GrabModeAsync,
				root, cursor, CurrentTime) != GrabSuccess)
		return NULL;

	XEvent ev = {0};
	while (ev.type != ButtonPress)
		XNextEvent(disp, &ev);

	int start_x = ev.xbutton.x;
	int start_y = ev.xbutton.y;
	XChangeActivePointerGrab(disp, PointerMotionMask | ButtonReleaseMask, cursor2, CurrentTime);

	while (display_X11_process_events(gc, start_x, start_y, area)) {
		fd_set fdset;
		FD_ZERO(&fdset);
		FD_SET(ConnectionNumber(disp), &fdset);
		select(ConnectionNumber(disp)+1, &fdset, NULL, NULL, NULL);
	}

	XUngrabPointer(disp, CurrentTime);
	XFreeCursor(disp, cursor);
	XFreeCursor(disp, cursor2);
	XFreeGC(disp, gc);
	XSync(disp, True);

	display_X11_area_sanitize(area);

	return area;
}

struct Area *display_X11_select_window(void)
{
	struct Area *area = calloc(1, sizeof(*area));

	Cursor cursor = XCreateFontCursor(disp, XC_left_ptr);

	if (XGrabPointer(disp, root, False, ButtonPressMask, GrabModeAsync,
				GrabModeAsync, root, cursor, CurrentTime) != GrabSuccess)
		return NULL;

	XEvent ev = {0};
	while (ev.type != ButtonPress)
		XNextEvent(disp, &ev);

	XUngrabPointer(disp, CurrentTime);
	XFreeCursor(disp, cursor);
	XSync(disp, True);

	Window target = ev.xbutton.subwindow;
	XWindowAttributes attr;
	XGetWindowAttributes(disp, target, &attr);
	area->width = attr.width;
	area->height = attr.height;
	Window child;
	XTranslateCoordinates(disp, target, root, 0, 0, &area->x, &area->y, &child);

	display_X11_area_sanitize(area);

	return area;
}
