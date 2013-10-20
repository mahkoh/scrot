#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdbool.h>

#include "scrot.h"

typedef void * Image;

void image_init(void);
Image image_from_screen(void);
Image image_from_area(struct Area *area);
Image image_concat(Image images[], int count);
Image image_scale(Image image, int cwidth, int cheight, int twidth, int theight);
void image_set_quality(Image image, int quality);
int image_width(Image image);
int image_height(Image image);
char *image_format(Image image);
bool image_save(Image image, char *filename);

#endif
