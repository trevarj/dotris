#include <stdlib.h>

#include <curses.h>

#include "dotris.h"

#ifdef DEBUG
#define ASSERT(expr)                                                                               \
    do {                                                                                           \
        if (!(expr))                                                                               \
            abort();                                                                               \
    } while (0)
#else
#define ASSERT(expr)
#endif

uint8_t const I_STATES[4][4][4] = {
    {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
};

uint8_t const J_STATES[4][4][4] = {
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}},
    {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 1, 1}},
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}},
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
};

uint8_t const L_STATES[4][4][4] = {
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}},
    {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}},
    {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}},
};

uint8_t const O_STATES[4][4][4] = {
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
};

uint8_t const S_STATES[4][4][4] = {
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}},
    {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}},
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}},
    {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}},
};

uint8_t const T_STATES[4][4][4] = {
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 1, 0}},
    {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}},
    {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}},
    {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 1, 0}},
};

uint8_t const Z_STATES[4][4][4] = {
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}},
    {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}},
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}},
    {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}},
};

// clang-format on

// Makes a new Tetrimino in the default position.
Tetrimino tetrimino_create(TetriminoType type) {
    Tetrimino new;
    new.type                   = type;
    new.state                  = FIRST_STATE;
    const uint8_t(*c)[4][4][4] = NULL;
    switch (type) {
    case I:
        c = &I_STATES;
        break;
    case J:
        c = &J_STATES;
        break;
    case L:
        c = &L_STATES;
        break;
    case O:
        c = &O_STATES;
        break;
    case S:
        c = &S_STATES;
        break;
    case T:
        c = &T_STATES;
        break;
    case Z:
        c = &Z_STATES;
        break;
    default:
        ASSERT(0 && "unreachable");
        break;
    }
    new.states = c;
    new.x      = 2;
    new.y      = 0;
    return new;
}

// Makes a random Tetrimino
Tetrimino tetrimino_create_random(void) {
    return tetrimino_create((TetriminoType)rand() % TETRIMINO_CNT);
}

TetriminoState tetrimino_next_state(TetriminoState state) {
    return (state == FOURTH_STATE) ? FIRST_STATE : state + 1;
}
