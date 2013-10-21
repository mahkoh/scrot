CFLAGS = -Wall -Wpedantic -Werror -std=c99

LDFLAGS = -lX11 -lImlib2

CC = clang

OBJECTS = help.o options.o display.o display_X11.o image.o image_X11.o utils.o

PREFIX ?= /usr/local

all: options.o help.o display.o image.o utils.o
	$(CC) $(CFLAGS) -o scrot scrot.c $(OBJECTS) $(LDFLAGS) 

display.o: display_X11.o
	$(CC) $(CFLAGS) -c display.c

display_X11.o: options.o
	$(CC) $(CFLAGS) -c display_X11.c

options.o: help.o
	$(CC) $(CFLAGS) -c options.c

help.o:
	$(CC) $(CFLAGS) -c help.c

utils.o: image.o
	$(CC) $(CFLAGS) -c utils.c

image.o: image_X11.o
	$(CC) $(CFLAGS) -c image.c

image_X11.o:
	$(CC) $(CFLAGS) -c image_X11.c

clean:
	rm -f *.png *.o scrot


install: all
	install -D scrot $(DESTDIR)$(PREFIX)/bin/scrot
