all: build

CFLAGS = -std=c18 -lncursesw -Iinclude/ -Wall -Werror -pedantic
ifeq ($(DEBUG), 1)
    CFLAGS += -g
endif
SOURCES = src/main.c src/grid.c src/tetriminos.c

build: $(SOURCES)
	$(CC) -o dotris $(CFLAGS) $(SOURCES)

clean: dotris
	rm dotris

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif
install: build
	install -D dotris $(DESTDIR)$(PREFIX)/bin/dotris
