CFLAGS   := -std=c18 -Wall -Wextra -Wpedantic
CPPFLAGS := -Iinclude
LDFLAGS  := `pkg-config --libs ncursesw`

VPATH := out src tests

OBJ_PATH := out/
SRC_PATH := src/
TST_PATH := tests/

MAIN := main.c
SRC := grid.c tetriminos.c
TST := grid_test.c
MAIN_OBJ := $(addprefix $(OBJ_PATH),$(MAIN:.c=.o))
OBJ := $(addprefix $(OBJ_PATH),$(SRC:.c=.o))
TST_OBJ := $(addprefix $(OBJ_PATH),$(TST:.c=.o))

PREFIX := /usr/local

ifeq ($(DEBUG), 1)
		CFLAGS += -Og -ggdb -fsanitize=address,undefined -DDEBUG -Werror
else
		CFLAGS += $(CFLAGS) -O2
endif

all: dotris

.PHONY: test clean install

$(OBJ_PATH)%.o: $(SRC_PATH)%.c | $(OBJ_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJ_PATH)%.o: $(TST_PATH)%.c | $(OBJ_PATH)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJ_PATH):
	@mkdir -p $@

dotris: $(OBJ) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

test: CFLAGS += -lcheck
test: $(TST_OBJ) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)
	./test

clean:
	rm -rf dotris test $(OBJ) $(OBJ_PATH)

install: dotris
	install -D dotris $(DESTDIR)$(PREFIX)/bin/dotris
