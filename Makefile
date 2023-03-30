CFLAGS   := -std=c18 -Wall -Werror -pedantic
CPPFLAGS := -Iinclude
LDFLAGS  := -lncursesw

VPATH := src

SRC := main.c grid.c tetriminos.c
OBJ := $(SRC:.c=.o)

PREFIX := /usr/local

ifeq ($(DEBUG), 1)
		CFLAGS += -Og -g
else
		CFLAGS += $(CFLAGS) -O2
endif

all: dotris

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $+

dotris: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

clean:
	rm -f dotris $(OBJ)

install: dotris
	install -D dotris $(DESTDIR)$(PREFIX)/bin/dotris
