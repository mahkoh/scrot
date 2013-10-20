#define	_GNU_SOURCE

#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

#include "utils.h"
#include "image.h"

static char *util_fmt_int(char *fmt, int n)
{      
	char *tmp;
	asprintf(&tmp, fmt, n);
	return tmp;
}

static char *util_fmt_char(char c, char *path_im, char *path_thumb, Image im)
{
	switch (c) {
	case 'f':
		return path_im;
	case 'm':
		return path_thumb;
	case 'n':
		if (path_im) {
			char *tmp = strrchr(path_im, '/');
			if (tmp)
				return tmp;
		}
		return path_im;
	case 'w':
		return util_fmt_int("%d", image_width(im));
	case 'h':
		return util_fmt_int("%d", image_height(im));
	case 's':
		if (path_im) {
			struct stat st;
			if (!stat(path_im, &st))
				return util_fmt_int("%d", st.st_size);
			else
				return "[err]";
		}
		return NULL;
	case 'p':
		return util_fmt_int("%d", image_width(im) * image_height(im));
	case 't':
		return image_format(im);
	case '$':
		return "$";
	default:
		return util_fmt_int("%c", c);
	}
}

char *util_fmt_str(char *str, struct tm *tm, char *path_im, char *path_thumb, Image im)
{
	size_t buff_len = 4092;

	char buf[buff_len];
	strftime(buf, buff_len-1, str, tm);

	int left = buff_len;
	char *ret = malloc(buff_len);
	ret[0] = '\0';

	for (char *c = buf; *c != '\0'; c++) {
		if (*c != '$' && *c != '\\') {
			if (left == 1) {
				left += buff_len;
				buff_len *= 2;
				ret = realloc(ret, buff_len);
			}
			ret[buff_len-left] = *c;
			left--;
			ret[buff_len-left] = '\0';
			continue;
		}

		char *tmp = NULL;
		if (*c == '$') {
			c++;
			tmp = util_fmt_char(*c, path_im, path_thumb, im); 
		} else if (*c == '\\') {
			c++;
			if (*c == 'n') {
				if (path_im != NULL)
					tmp = "\n";
			} else {
				tmp = util_fmt_int("%c", *c);
			}
		}
		
		if (tmp != NULL) {
			int len = strlen(tmp);
			while (left < len + 1) {
				left += buff_len;
				buff_len *= 2;
				ret = realloc(ret, buff_len);
			}
			left -= len;
			strcat(ret, tmp);
		}
	}

	return ret;
}
