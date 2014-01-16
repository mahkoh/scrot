#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <stdbool.h>

struct Options {
	const char *output_file; 
	const char *thumb_file;  
	const char *exec;        
	int         debug_level;  
	int         delay;        
	int         quality;      
	int         thumb;        
	int         thumb_width;  
	int         thumb_height; 
	bool        countdown;    
	bool        select;       
	bool        window;       
	bool        border;       
	bool        multidisp;    
};

extern struct Options *opt;

void options_init(int argc, char **argv);

#endif
