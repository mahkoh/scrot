#define	_GNU_SOURCE

#include <getopt.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "options.h"
#include "help.h"

struct Options *opt;

static char *options_name_thumb(char *name)
{
	char *thumb = malloc(strlen(name) + sizeof("-thumb"));
	char *dot = strrchr(name, '.');
	if (dot != NULL) {
		strncpy(thumb, name, dot - name);
		strcat(thumb, "-thumb");
		strcat(thumb, dot);
	} else {
		sprintf(thumb, "%s-thumb", name);
	}
	return thumb;
}

static void options_init_thumbnail(struct Options *tmp_opt, char *optarg)
{
	if (strchr(optarg, 'x')) {
		// We want to specify the geometry
		char *tok = strtok(optarg, "x");
		tmp_opt->thumb_width = atoi(tok);
		tok = strtok(NULL, "x");
		if (tok) {
			tmp_opt->thumb_width = atoi(optarg);
			tmp_opt->thumb_height = atoi(tok);

			if (tmp_opt->thumb_width < 0) {
				tmp_opt->thumb_width = 1;
			}
			if (tmp_opt->thumb_height < 0) {
				tmp_opt->thumb_height = 1;
			}

			if (tmp_opt->thumb_width == 0 && tmp_opt->thumb_height == 0) {
				tmp_opt->thumb = 0;
			}
			else {
				tmp_opt->thumb = 1;
			}
		}
	} else {
		tmp_opt->thumb = atoi(optarg);
		if (tmp_opt->thumb < 1) {
			tmp_opt->thumb = 1;
		} else if (tmp_opt->thumb > 100) {
			tmp_opt->thumb = 100;
		}
	}
}

void options_init(int argc, char **argv)
{
	struct Options *tmp_opt = calloc(1, sizeof(*tmp_opt));

	tmp_opt->quality = 75;

	char *stropts = "bcd:e:hmq:swt:v+:";
	struct option lopts[] = {
		// actions
		{"help", 0, 0, 'h'},                  // okay
		{"version", 0, 0, 'v'},               // okay
		{"count", 0, 0, 'c'},
		{"select", 0, 0, 's'},
		{"window", 0, 0, 'w'},
		{"border", 0, 0, 'b'},
		{"multidisp", 0, 0, 'm'},
		// toggles
		{"thumb", 1, 0, 't'},
		{"delay", 1, 0, 'd'},
		{"quality", 1, 0, 'q'},
		{"exec", 1, 0, 'e'},
		{"debug-level", 1, 0, '+'},
		{0, 0, 0, 0}
	};
	int optch = 0;
	int cmdx = 0;

	// Now to pass some optionarinos
	while ((optch = getopt_long(argc, argv, stropts, lopts, &cmdx)) != EOF) {
		switch (optch) {
		case 0:
			break;
		case 'h':
			help_show_usage();
			break;
		case 'v':
			help_show_version();
			break;
		case 'b':
			tmp_opt->border = true;
			break;
		case 'd':
			tmp_opt->delay = atoi(optarg);
			break;
		case 'e':
			tmp_opt->exec = strcpy(malloc(strlen(optarg) + 1), optarg);
			break;
		case 'm':
			tmp_opt->multidisp = true;
			break;
		case 'q':
			tmp_opt->quality = atoi(optarg);
			break;
		case 's':
			tmp_opt->select = true;
			break;
		case 'w':
			tmp_opt->window = true;
			break;
		case '+':
			tmp_opt->debug_level = atoi(optarg);
			break;
		case 'c':
			tmp_opt->countdown = true;
			break;
		case 't':
			options_init_thumbnail(tmp_opt, optarg);
			break;
		default:
			break;
		}
	}

	// Now the leftovers, which must be files
	if (optind < argc) {
		while (optind < argc) {
			// If recursive is NOT set, but the only argument is a directory
			// name, we grab all the files in there, but not subdirs
			if (tmp_opt->output_file == NULL) {
				tmp_opt->output_file = argv[optind++];
				if (tmp_opt->thumb) {
					tmp_opt->thumb_file = options_name_thumb(opt->output_file);
				}
			} else {
				fprintf(stderr, "unrecognised option %s\n", argv[optind++]);
				exit(EXIT_FAILURE);
			}
		}
	}
	// So that we can safely be called again
	optind = 1;

	if (tmp_opt->output_file == NULL) {
		tmp_opt->output_file = strdup("%Y-%m-%d-%H%M%S_$wx$h_scrot.png");
		tmp_opt->thumb_file = strdup("%Y-%m-%d-%H%M%S_$wx$h_scrot-thumb.png");
	}

	opt = tmp_opt;
}

