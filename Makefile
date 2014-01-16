CFLAGS = -Wall -Wpedantic -Werror -std=c11

LDFLAGS = -lX11 -lImlib2

SRC = scrot.c help.c options.c display.c display_X11.c image.c image_X11.c utils.c
HED = $(SRC:.c=.h)
OBJ = $(SRC:.c=.o)

PREFIX ?= /usr/local

all: scrot

scrot: $(OBJ)
	$(CC) $(CFLAGS) -o scrot $(OBJ) $(LDFLAGS) 

${OBJ}: ${HED}

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.png *.o scrot

install: all
	install -D scrot $(DESTDIR)$(PREFIX)/bin/scrot
