CFLAGS   := -std=c18 -Wall -Werror -pedantic
CPPFLAGS := -Iinclude
LDFLAGS  := -lncursesw

VPATH := build src

OBJ_PATH := out/
SRC_PATH := src/

SRC := main.c grid.c tetriminos.c
OBJ := $(addprefix $(OBJ_PATH),$(SRC:.c=.o))

PREFIX := /usr/local

ifeq ($(DEBUG), 1)
		CFLAGS += -Og -g
else
		CFLAGS += $(CFLAGS) -O2
endif

all: dotris

$(OBJ_PATH)%.o: $(SRC_PATH)%.c | $(OBJ_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJ_PATH):
	@mkdir -p $@

dotris: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

clean:
	rm -rf dotris $(OBJ) $(OBJ_PATH)

install: dotris
	install -D dotris $(DESTDIR)$(PREFIX)/bin/dotris
