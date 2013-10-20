#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "options.h"
#include "display.h"
#include "image.h"
#include "utils.h"

static void scrot_delay(void)
{
	if (opt->delay > 0) {
		if (opt->countdown) {
			printf("Taking shot in %d.. ", opt->delay);
			fflush(stdout);
			sleep(1);
			for (int i = opt->delay - 1; i > 0; i--) {
				printf("%d.. ", i);
				fflush(stdout);
				sleep(1);
			}
			printf("0.\n");
		} else {
			sleep(opt->delay);
		}
	}
}

static void scrot_exec(Image const image, struct tm const *tm, char const *filename_im, char const *filename_thumb)
{
	char const *execstr = format_filename(opt->exec, tm, filename_im, filename_thumb, image);
	system(execstr);
	exit(EXIT_SUCCESS);
}

static Image scrot_shoot_all_screens(void)
{
	int screens = display_num_screens();
	if (screens < 2)
		return image_from_screen();

	Image images[screens];
	for (int i = 0; i < screens; i++) {
		display_open_screen(i);
		images[i] = image_from_screen();
	}

	return image_concat(images, screens);
}

static Image scrot_shoot(void)
{
	if (opt->select || opt->window) {
		struct Area area;
		if (opt->select)
			area = display_select_area();
		else
			area = display_select_window();
		return image_from_area(&area);
	}
	scrot_delay();
	if (opt->multidisp)
		return scrot_shoot_all_screens();
	return image_from_screen();
}

static Image scrot_create_thumbnail(Image image)
{
	int cwidth = image_width(image);
	int cheight = image_height(image);
	int twidth;
	int theight;

	if (opt->thumb_width > 0 || opt->thumb_height > 0) {
		if (opt->thumb_width == 0) {
			twidth = cwidth * opt->thumb_height / cheight;
			theight = opt->thumb_height;
		} else if (opt->thumb_height == 0) {
			twidth = opt->thumb_width;
			theight = cheight * opt->thumb_width / cwidth;
		} else {
			twidth = opt->thumb_width;
			theight = opt->thumb_height;
		}
	} else {
		twidth = cwidth * opt->thumb / 100;
		theight = cheight * opt->thumb / 100;
	}

	return image_scale(image, cwidth, cheight, twidth, theight);
}

int main(int argc, char **argv)
{
	options_init(argc, argv);
	display_init();
	image_init();

	Image image = scrot_shoot();
	if (image == NULL) {
		fprintf(stderr, "no image grabbed\n");
		return 1;
	}

	image_set_quality(image, opt->quality);

	time_t t;
	time(&t);
	struct tm *tm = localtime(&t);

	char *filename_image = format_filename(opt->output_file, tm, NULL, NULL, image);
	char *filename_thumb = NULL;

	bool error = image_save(image, filename_image);
	if (error) {
		fprintf(stderr, "Saving to file %s failed\n", filename_image);
		return 1;
	}

	if (opt->thumb != 0) {
		Image thumbnail = scrot_create_thumbnail(image);
		if (thumbnail == NULL) {
			fprintf(stderr, "Unable to create scaled Image\n");
			return 1;
		}

		filename_thumb = format_filename(opt->thumb_file, tm, NULL, NULL, thumbnail);
		error = image_save(thumbnail, filename_thumb);
		if (error) {
			fprintf(stderr, "Saving thumbnail %s failed\n", filename_thumb);
			return 1;
		}
	}

	if (opt->exec != NULL)
		scrot_exec(image, tm, filename_image, filename_thumb);
}
