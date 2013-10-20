#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdbool.h>

#include "scrot.h"

typedef void * Image;

void image_init();
Image image_from_screen();
Image image_from_area(struct Area *area);
Image image_concat(Image const images[], int count);
Image image_scale(Image image, int cwidth, int cheight, int twidth, int theight);
void image_set_quality(Image image, int quality);
int image_width(Image image);
int image_height(Image image);
Image image_new(int width, int height);
bool image_save(Image image, char const *filename);

#endif
