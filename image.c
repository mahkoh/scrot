#include <Imlib2.h>
#include <string.h>

#include "image.h"
#include "image_X11.h"

void image_init(void)
{
	image_X11_init();
}

Image image_from_screen(void)
{
	return image_X11_from_screen();
}

Image image_from_area(struct Area *area)
{
	return image_X11_from_area(area);
}

int image_width(Image image)
{
	imlib_context_set_image(image);
	return imlib_image_get_width();
}

int image_height(Image image)
{
	imlib_context_set_image(image);
	return imlib_image_get_height();
}

Image image_concat(Image images[], int count)
{
	int total_width = 0;
	int max_height = 0;
	int widths[count];
	int heights[count];

	for (int i = 0; i < count; i++) {
		imlib_context_set_image(images[i]);
		heights[i] = imlib_image_get_height();
		widths[i] = imlib_image_get_width();
		max_height = heights[i] > max_height ? heights[i] : max_height;
		total_width += widths[i];
	}

	Imlib_Image ret = imlib_create_image(total_width, max_height);
	imlib_context_set_image(ret);
	imlib_context_set_color(255, 0, 0, 0);
	imlib_image_fill_rectangle(0, 0, total_width, max_height);

	imlib_context_set_anti_alias(0);
	imlib_context_set_dither(1);
	imlib_context_set_blend(0);
	imlib_context_set_angle(0);

	int x = 0;
	for (int i = 0; i < count; i++) {
		imlib_blend_image_onto_image(images[i], 0, 0, 0, widths[i], heights[i], x, 0, widths[i], heights[i]);
		x += widths[i];
	}

	return ret;
}

bool image_save(Image image, char *filename)
{
	char *tmp = strrchr(filename, '.');
	if (tmp) {
		imlib_image_set_format(tmp + 1);
	}

	Imlib_Load_Error err;
	imlib_context_set_image(image);
	imlib_save_image_with_error_return(filename, &err);

	return err != IMLIB_LOAD_ERROR_NONE;
}

Image image_scale(Image image, int cwidth, int cheight, int twidth, int theight)
{
	imlib_context_set_image(image);
	imlib_context_set_anti_alias(1);
	return imlib_create_cropped_scaled_image(0, 0, cwidth, cheight, twidth, theight);
}

void image_set_quality(Image image, int quality)
{
	imlib_context_set_image(image);
	imlib_image_attach_data_value("quality", NULL, quality, NULL);
}

char *image_format(Image image)
{
	imlib_context_set_image(image);
	return imlib_image_format();
}
