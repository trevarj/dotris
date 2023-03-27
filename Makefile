all: dotris

CFLAGS = -lncursesw -Iinclude/
SOURCES = src/main.c src/grid.c src/tetriminos.c

dotris: $(SOURCES)
	$(CC) -o dotris $(CFLAGS) $(SOURCES)

clean: dotris
	rm dotris

install: dotris
	mv dotris /usr/local/bin/dotris
