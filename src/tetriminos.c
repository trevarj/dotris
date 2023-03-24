#include <curses.h>
#include <grid.h>
#include <stdio.h>
#include <stdlib.h>

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

extern uint8_t GRID[GRID_HEIGHT][GRID_WIDTH];

// Makes a new Tetrimino in the default position.
Tetrimino make_tetrimino(TetriminoType type) {
    Tetrimino new;
    new.type = type;
    new.state = First;
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

int is_valid_transformation(const uint8_t (*state)[4][4], int x, int y) {
    for (int i = 0; i < 4; i++) {
        int rel_y = y + i;
        for (int j = 0; j < 4; j++) {
            int rel_x = x + j;
            if ((*state)[i][j]) {
                if (rel_x < 0 || rel_y < 0 || rel_x >= GRID_WIDTH || rel_y >= GRID_HEIGHT ||
                    GRID[rel_y][rel_x]) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

MoveResult move_tetrimino(Tetrimino *t, Direction d) {
    int temp_x = t->x, temp_y = t->y;
    MoveResult res = Success;
    switch (d) {
    case Left:
        temp_x -= 1;
        res = FailedH;
        break;
    case Right:
        temp_x += 1;
        res = FailedH;
        break;
    case Down:
        temp_y += 1;
        res = FailedV;
        break;
    }
    if (is_valid_transformation(&(*t->states)[t->state], temp_x, temp_y)) {
        t->x = temp_x;
        t->y = temp_y;
        return Success;
    }
    return res;
}

TetriminoState next_state(TetriminoState state) { return (state == Fourth) ? First : state + 1; }

// Alters the state of the tetrimino if possible. Rotates clockwise.
void rotate_tetrimino(Tetrimino *t) {
    TetriminoState n = next_state(t->state);
    if (is_valid_transformation(&(*t->states)[n], t->x, t->y)) {
        t->state = n;
    }
}

// Hard drop and return rows covered
int drop_tetrimino(Tetrimino *t) {
    MoveResult res = Success;
    int rows = 0;
    while (res != FailedV) {
        res = move_tetrimino(t, Down);
        rows++;
    }
    return rows;
}

// Locks Tetrimino into GRID
inline void write_to_grid(Tetrimino *t) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            GRID[t->y + j][t->x + i] |= (*t->states)[t->state][j][i];
        }
    }
}

// Removes Tetrimino from GRID
inline void remove_from_grid(Tetrimino *t) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            /* if ((*t->states)[t->state][j][i]) */
            /*     GRID[t->y + j][t->x + i] = 0; */
            GRID[t->y + j][t->x + i] ^= (*t->states)[t->state][j][i];
        }
    }
}
