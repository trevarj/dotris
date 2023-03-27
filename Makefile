all: dotris

CFLAGS = -lncursesw -Iinclude/
SOURCES = src/main.c src/grid.c src/tetriminos.c

dotris: $(SOURCES)
	$(CC) -o dotris $(CFLAGS) $(SOURCES)

clean: dotris
	rm dotris
