CFLAGS = -Wall -Wextra -Wpedantic -Wformat -Wunused -Wno-unused-parameter -Wuninitialized \
		-Wwrite-strings -Wstrict-prototypes -Wold-style-definition -Wredundant-decls -Wnested-externs \
		-Werror -std=c99 -Wno-missing-field-initializers

LDFLAGS = -lX11 -lImlib2

CC = clang

OBJECTS = help.o options.o display.o image.o utils.o

DESTDIR ?= /usr/local/bin

all: options.o help.o display.o image.o utils.o
	$(CC) $(CFLAGS) -o scrot scrot.c $(OBJECTS) $(LDFLAGS) 

display.o: options.o
	$(CC) $(CFLAGS) -c display.c

options.o: help.o
	$(CC) $(CFLAGS) -c options.c

help.o:
	$(CC) $(CFLAGS) -c help.c

utils.o: image.o
	$(CC) $(CFLAGS) -c utils.c

image.o:
	$(CC) $(CFLAGS) -c image.c


clean:
	rm -f *.png *.o scrot


install: all
	install scrot $(DESTDIR)/scrot
