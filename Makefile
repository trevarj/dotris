.POSIX:
.SUFFIXES:

CC       := cc
CFLAGS   := -std=c99 -D_POSIX_C_SOURCE=199309L -Wall -Wextra -Wpedantic -O2
# Debug Mode
# CFLAGS += -Og -ggdb -fsanitize=address,undefined -DDEBUG -Werror
LDFLAGS  := `pkg-config --libs ncursesw`

PREFIX := /usr/local

INSTALL_DIR := $(DESTDIR)$(PREFIX)/bin/
MAN_DIR := $(DESTDIR)$(PREFIX)/man/man6/

OBJECTS = main.o grid.o tetriminos.o
TEST_OBJECTS = grid_test.o grid.o tetriminos.o

all: dotris

.PHONY: test clean install install-man

dotris: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)

.SUFFIXES: .c .o
.c.o: $(OUT_DIR)
	$(CC) $(CFLAGS) -c $<

test: CFLAGS += `pkg-config --cflags check`
test: LDFLAGS += `pkg-config --libs check`
test: $(TEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $? $(LDFLAGS)
	./test

clean:
	rm -f dotris dotris.6.gz test *.o

install-man:
	gzip -fk dotris.6
	mkdir -p $(MAN_DIR)
	install dotris.6.gz $(MAN_DIR)

install: dotris install-man
	mkdir -p $(INSTALL_DIR)
	install dotris $(INSTALL_DIR)
