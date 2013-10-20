#ifndef _UTILS_H_
#define _UTILS_H_

#include <time.h>

#include "image.h"

char *format_filename(char const *str, struct tm const *tm, char const *filename_im, char const *filename_thumb, Image im);

#endif
