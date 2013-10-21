#ifndef _UTILS_H_
#define _UTILS_H_

#include <time.h>

#include "image.h"

char *util_fmt_str(char *str, struct tm *tm, char *path_im, char *path_thumb, Image im);
void util_error(char *fmt, ...);

#endif
