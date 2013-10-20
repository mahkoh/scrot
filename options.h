#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <stdbool.h>

struct Options {
	int	debug_level;
	int	delay;
	bool	countdown;
	bool	select;
	bool	window;
	int	quality;
	bool	border;
	bool	multidisp;
	int	thumb;
	int	thumb_width;
	int	thumb_height;
	char 	*output_file;
	char	*thumb_file;
	char 	*exec;
};

extern struct Options *opt;

void options_init(int argc, char **argv);

#endif
