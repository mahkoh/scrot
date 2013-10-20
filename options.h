#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <stdbool.h>

struct Options {
	int		debug_level;
	int		delay;
	bool		countdown;
	bool		select;
	bool		window;
	int		quality;
	bool		border;
	bool		multidisp;
	int		thumb;
	int		thumb_width;
	int		thumb_height;
	char const 	*output_file;
	char const	*thumb_file;
	char const 	*exec;
};

extern struct Options const *opt;

void options_init(int argc, char **argv);

#endif
