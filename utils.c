#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

#include "utils.h"
#include "image.h"

char *format_filename(char const *str, struct tm const *tm, char const *filename_im, char const *filename_thumb, Image im)
{
	char *c;
	char buf[20];
	char ret[4096];
	char strf[4096];
	char *tmp;
	struct stat st;

	ret[0] = '\0';
	strftime(strf, 4095, str, tm);

	for (c = strf; *c != '\0'; c++) {
		if (*c == '$') {
			c++;
			switch (*c) {
				case 'f':
					if (filename_im)
						strcat(ret, filename_im);
					break;
				case 'm': /* t was allready taken, so m as in mini */
					if (filename_thumb)
						strcat(ret, filename_thumb);
					break;
				case 'n':
					if (filename_im) {
						tmp = strrchr(filename_im, '/');
						if (tmp)
							strcat(ret, tmp + 1);
						else
							strcat(ret, filename_im);
					}
					break;
				case 'w':
					snprintf(buf, sizeof(buf), "%d", image_width(im));
					strcat(ret, buf);
					break;
				case 'h':
					snprintf(buf, sizeof(buf), "%d", image_height(im));
					strcat(ret, buf);
					break;
				case 's':
					if (filename_im) {
						if (!stat(filename_im, &st)) {
							int size;

							size = st.st_size;
							snprintf(buf, sizeof(buf), "%d", size);
							strcat(ret, buf);
						} else
							strcat(ret, "[err]");
					}
					break;
				case 'p':
					snprintf(buf, sizeof(buf), "%d", image_width(im) * image_height(im));
					strcat(ret, buf);
					break;
				/*case 't':
					strcat(ret, imlib_image_format());
					break; */
				case '$':
					strcat(ret, "$");
					break;
				default:
					strncat(ret, c, 1);
					break;
			}
		} else if (*c == '\\') {
			c++;
			switch (*c) {
				case 'n':
					if (filename_im)
						strcat(ret, "\n");
					break;
				default:
					strncat(ret, c, 1);
					break;
			}
		} else
			strncat(ret, c, 1);
	}
	char *rv = malloc(strlen(ret) + 1);
	strcpy(rv, ret);
	return rv;
}
