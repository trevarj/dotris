#include <stdlib.h>

#include <curses.h>

#include "tetriminos.h"

// clang-format off

uint8_t const I_STATES[4][4][4] = {
  {
    {0, 0, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 0}
  },
  {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
  {
    {0, 0, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 0}
  },
  {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
};

uint8_t const J_STATES[4][4][4] = {
  {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 1, 1, 1},
    {0, 0, 0, 1}
  },
  {
    {0, 0, 0, 0},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {0, 0, 1, 1}
  },
  {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 1}
  },
  {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0}
  },
};

uint8_t const L_STATES[4][4][4] = {
  {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {1, 1, 1, 0},
    {1, 0, 0, 0}
  },
  {
    {0, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0}
  },
  {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 1, 0},
    {1, 1, 1, 0}
  },
  {
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0}
  },
};

uint8_t const O_STATES[4][4][4] = {
  {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
  },
  {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
  },
  {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
  },
  {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
  },
};

uint8_t const S_STATES[4][4][4] = {
  {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 1, 1},
    {0, 1, 1, 0}
  },
  {
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0}
  },
  {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 1, 1},
    {0, 1, 1, 0}
  },
  {
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0}
  },
};

uint8_t const T_STATES[4][4][4] = {
  {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 1, 1, 1},
    {0, 0, 1, 0}
  },
  {
    {0, 0, 0, 0},
    {0, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0}
  },
  {
    {0, 0, 0, 0},
    {0, 0, 1, 0},
    {0, 1, 1, 1},
    {0, 0, 0, 0}
  },
  {
    {0, 0, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 1, 0}
  },
};

uint8_t const Z_STATES[4][4][4] = {
  {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 1, 0}
  },
  {
    {0, 0, 0, 0},
    {0, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 1, 0, 0}
  },
  {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 1, 0}
  },
  {
    {0, 0, 0, 0},
    {0, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 1, 0, 0}
  },
};

// clang-format on

// Makes a new Tetrimino in the default position.
Tetrimino make_tetrimino(TetriminoType type) {
    Tetrimino new;
    new.type = type;
    new.state = FIRST_STATE;
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
    }
    new.states = c;
    new.x = 2;
    new.y = 0;
    return new;
}

// Makes a random Tetrimino
Tetrimino random_tetrimino(void) { return make_tetrimino((TetriminoType)rand() % 7); }

TetriminoState next_state(TetriminoState state) {
    return (state == FOURTH_STATE) ? FIRST_STATE : state + 1;
}
