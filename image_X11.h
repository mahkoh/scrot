#ifndef _IMAGE_X11_H_
#define _IMAGE_X11_H_

#include <stdbool.h>

#include "scrot.h"
#include "image.h"

void image_X11_init(void);
Image image_X11_from_screen(void);
Image image_X11_from_area(struct Area *area);

#endif
