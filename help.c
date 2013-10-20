#include <stdio.h>
#include <stdlib.h>

#include "help.h"

#define PACKAGE "scrot"
#define VERSION "1"

void help_show_version(void)
{
	printf(PACKAGE " version " VERSION "\n");
	exit(EXIT_SUCCESS);
}

void help_show_mini_usage(void)
{
	printf("Usage : " PACKAGE " [OPTIONS]... FILE\nUse " PACKAGE " --help for detailed usage information\n");
	exit(EXIT_SUCCESS);
}

void help_show_usage(void)
{
	fprintf(stdout,
			"Usage : " PACKAGE " [OPTIONS]... [FILE]\n"
			"  Where FILE is the target file for the screenshot.\n"
			"  If FILE is not specified, a date-stamped file will be dropped in the\n"
			"  current directory.\n" "  See man " PACKAGE " for more details\n"
			"  -h, --help                display this help and exit\n"
			"  -v, --version             output version information and exit\n"
			"  -b, --border              When selecting a window, grab wm border too\n"
			"  -c, --count               show a countdown before taking the shot\n"
			"  -d, --delay NUM           wait NUM seconds before taking a shot\n"
			"  -e, --exec APP            run APP on the resulting screenshot\n"
			"  -q, --quality NUM         Image quality (1-100) high value means\n"
			"                            high size, low compression. Default: 75.\n"
			"                            For lossless compression formats, like png,\n"
			"                            low quality means high compression.\n"
			"  -m, --multidisp           For multiple heads, grab shot from each\n"
			"                            and join them together.\n"
			"  -s, --select              interactively choose a rectnagle with the mouse\n"
			"  -w, --window              interactively choose a window with the mouse\n"
			"  -t, --thumb NUM           generate thumbnail too. NUM is the percentage\n"
			"                            of the original size for the thumbnail to be,\n"
			"                            or the geometry in percent, e.g. 50x60 or 80x20.\n"
			"\n" "  SPECIAL STRINGS\n"
			"  Both the --exec and filename parameters can take format specifiers\n"
			"  that are expanded by " PACKAGE " when encountered.\n"
			"  There are two types of format specifier. Characters preceded by a '%%'\n"
			"  are interpretted by strftime(2). See man strftime for examples.\n"
			"  These options may be used to refer to the current date and time.\n"
			"  The second kind are internal to " PACKAGE
			"  and are prefixed by '$'\n"
			"  The following specifiers are recognised:\n"
			"                  $f image path/filename (ignored when used in the filename)\n"
			"                  $m thumbnail path/filename\n"
			"                  $n image name (ignored when used in the filename)\n"
			"                  $s image size (bytes) (ignored when used in the filename)\n"
			"                  $p image pixel size\n"
			"                  $w image width\n"
			"                  $h image height\n"
			"                  $t image format\n"
			"                  $$  prints a literal '$'\n"
			"                  \\n prints a newline (ignored when used in the filename)\n"
			"  Example:\n" "          " PACKAGE
			" '%%Y-%%m-%%d_$wx$h_scrot.png' -e 'mv $f ~/images/shots/'\n"
			"          Creates a file called something like 2000-10-30_2560x1024_scrot.png\n"
			"          and moves it to your images directory.\n" "\n"
			"This program is free software see the file COPYING for licensing info.\n"
			"Copyright Tom Gilbert 2000\n"
			"Email bugs to <scrot_sucks@linuxbrit.co.uk>\n");
	exit(EXIT_SUCCESS);
}
