#include <stdio.h>
#include <sys/select.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>

#include "scrot.h"
#include "display.h"
#include "display_X11.h"
#include "options.h"

Display *disp;
Screen *scr;
Window root;

void display_init(void)
{
	disp = XOpenDisplay(NULL);
	if (disp == NULL) {
		fprintf(stderr, "Can't open X display.\n");
		exit(EXIT_FAILURE);
	}
	scr = ScreenOfDisplay(disp, DefaultScreen(disp));
}

int display_num_screens(void)
{
	return ScreenCount(disp);
}

void display_open_screen(int num)
{
	scr = ScreenOfDisplay(disp, num);
}

static inline bool display_process_events(GC gc, int start_x, int start_y, struct Area *area)
{
	XEvent ev;

	while (XPending(disp)) {
		XNextEvent(disp, &ev);
		switch (ev.type) {
		case MotionNotify:
			XDrawRectangle(disp, root, gc, area->x, area->y, area->width, area->height);

			area->x = start_x;
			area->y = start_y;
			area->width = ev.xmotion.x - area->x;
			area->height = ev.xmotion.y - area->y;

			if (area->width < 0) {
				area->x += area->width;
				area->width = 0 - area->width;
			}
			if (area->height < 0) {
				area->y += area->height;
				area->height = 0 - area->height;
			}

			XDrawRectangle(disp, root, gc, area->x, area->y, area->width, area->height);
			XFlush(disp);
			break;
		case ButtonRelease:
			XDrawRectangle(disp, root, gc, area->x, area->y, area->width, area->height);
			return false;
		}
	}

	return true;
}

static void display_area_sanitize(struct Area *area)
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

struct Area display_select_area()
{
	struct Area area = {0};

	Cursor cursor = XCreateFontCursor(disp, XC_left_ptr);
	Cursor cursor2 = XCreateFontCursor(disp, XC_lr_angle);

	XGCValues gcval;
	gcval.foreground = XWhitePixel(disp, 0);
	gcval.function = GXxor;
	gcval.background = XBlackPixel(disp, 0);
	gcval.plane_mask = gcval.background ^ gcval.foreground;
	gcval.subwindow_mode = IncludeInferiors;

	root = RootWindow(disp, XScreenNumberOfScreen(scr));
	GC gc = XCreateGC(disp, root, GCFunction | GCForeground | GCBackground | GCSubwindowMode, &gcval);

	if (XGrabPointer(disp, root, False, ButtonReleaseMask, GrabModeAsync, GrabModeAsync, root, cursor, CurrentTime) != GrabSuccess) {
		fprintf(stderr, "couldn't grab pointer: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	XEvent ev = {0};
	while (ev.type != ButtonRelease)
		XNextEvent(disp, &ev);

	int start_x = ev.xbutton.x;
	int start_y = ev.xbutton.y;
	XChangeActivePointerGrab(disp, PointerMotionMask | ButtonReleaseMask, cursor2, CurrentTime);

	while (display_process_events(gc, start_x, start_y, &area)) {
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

	display_area_sanitize(&area);

	return area;
}

struct Area display_select_window()
{
	struct Area area;

	root = RootWindow(disp, XScreenNumberOfScreen(scr));
	Cursor cursor = XCreateFontCursor(disp, XC_left_ptr);

	if (XGrabPointer(disp, root, False, ButtonPressMask, GrabModeAsync,
				GrabModeAsync, root, cursor, CurrentTime) != GrabSuccess) {
		fprintf(stderr, "couldn't grab pointer: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	XEvent ev = {0};
	while (ev.type != ButtonPress)
		XNextEvent(disp, &ev);

	XUngrabPointer(disp, CurrentTime);
	XFreeCursor(disp, cursor);
	XSync(disp, True);

	Window target = ev.xbutton.subwindow;
	XWindowAttributes attr;
	XGetWindowAttributes(disp, target, &attr);
	area.width = attr.width;
	area.height = attr.height;
	Window child;
	XTranslateCoordinates(disp, target, root, 0, 0, &area.x, &area.y, &child);

	display_area_sanitize(&area);

	return area;
}
