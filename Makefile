all: build

CFLAGS = -lncursesw -Iinclude/
SOURCES = src/main.c src/grid.c src/tetriminos.c

build: $(SOURCES)
	$(CC) -o dotris $(CFLAGS) $(SOURCES)

clean: build
	rm dotris

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif
install: build
	install -D dotris $(DESTDIR)$(PREFIX)/bin/dotris
