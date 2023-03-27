#include <curses.h>
#include <dots.h>
#include <grid.h>
#include <string.h>

#define MVPRINTW(Y, X, STR) mvprintw(Y, X, "%s", STR)

uint8_t GRID[GRID_HEIGHT][GRID_WIDTH]; // The 10 x 22 gameboard

void print_grid(void) {
#ifndef DEBUG
#else
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            mvprintw(y, 10 + x, "%d", GRID[y][x]);
        }
    }
#endif
}

void draw_border(void) {
    MVPRINTW(GRID_BORDER_START_Y, GRID_BORDER_START_X, GRID_BORDER_TOP_LEFT);
    for (int x = GRID_START_X; x < GRID_BORDER_END_X; x++)
        MVPRINTW(GRID_BORDER_START_Y, x, GRID_BORDER_HORIZONTAL);
    MVPRINTW(GRID_BORDER_START_Y, GRID_BORDER_END_X, GRID_BORDER_TOP_RIGHT);

    for (int y = GRID_START_Y; y < GRID_BORDER_END_Y; y++) {
        MVPRINTW(y, GRID_BORDER_START_X, GRID_BORDER_VERTICAL);
        MVPRINTW(y, GRID_BORDER_END_X, GRID_BORDER_VERTICAL);
    }

    MVPRINTW(GRID_BORDER_END_Y, GRID_BORDER_START_X, GRID_BORDER_BOT_LEFT);
    for (int x = GRID_START_X; x < GRID_BORDER_END_X; x++)
        MVPRINTW(GRID_BORDER_END_Y, x, GRID_BORDER_HORIZONTAL);
    MVPRINTW(GRID_BORDER_END_Y, GRID_BORDER_END_X, GRID_BORDER_BOT_RIGHT);

    MVPRINTW(GRID_BORDER_START_Y - 1, GRID_BORDER_START_X - 1, TITLE);
}

// Draws the grid and imposes the tetrimino onto it temporarily
void draw_grid(Tetrimino *t) {
    // Add current piece to grid
    write_to_grid(t);

    int x = 0, y = GRID_HIDDEN_ROWS;
    for (int row = GRID_START_Y; row <= GRID_END_Y; row++) {
        for (int col = GRID_START_X; col <= GRID_END_X; col++) {
            int index = (GRID[y + 3][x + 1] << 7) | (GRID[y + 3][x] << 6) |
                        (GRID[y + 2][x + 1] << 5) | (GRID[y + 1][x + 1] << 4) |
                        (GRID[y][x + 1] << 3) | (GRID[y + 2][x] << 2) | (GRID[y + 1][x] << 1) |
                        (GRID[y][x]);
            MVPRINTW(row, col, DOTS[index]);
            x += CELL_WIDTH;
        }
        x = 0;
        y += CELL_HEIGHT;
    }
    print_grid();
    // Remove current piece from grid
    remove_from_grid(t);
}

// Checks if the given tetrimino state and position (x,y) can be placed on the GRID
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

// Locks Tetrimino into GRID
inline void write_to_grid(Tetrimino *t) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            GRID[t->y + j][t->x + i] |= (*t->states)[t->state][j][i];
        }
    }
}

MoveResult move_tetrimino(Tetrimino *t, Direction d) {
    int temp_x = t->x, temp_y = t->y;
    MoveResult res = Success;
    switch (d) {
    case Left:
        temp_x -= 1;
        res = HitSide;
        break;
    case Right:
        temp_x += 1;
        res = HitSide;
        break;
    case Down:
        temp_y += 1;
        res = HitBottom;
        break;
    }
    if (is_valid_transformation(&(*t->states)[t->state], temp_x, temp_y)) {
        t->x = temp_x;
        t->y = temp_y;
        return Success;
    }
    return res;
}

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
    while (res != HitBottom) {
        res = move_tetrimino(t, Down);
        rows++;
    }
    return rows;
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

int clear_lines(void) {
    int cleared = 0;
    for (int i = GRID_HEIGHT - 1; i >= GRID_HIDDEN_ROWS; i--) {
        int clear = 1;
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (!GRID[i][j]) {
                clear = 0;
                break;
            }
        }
        if (clear) {
            for (int y = i; y >= GRID_HIDDEN_ROWS; y--) {
                for (int x = 0; x < GRID_WIDTH; x++)
                    GRID[y][x] = GRID[y - 1][x];
            }
            cleared++;
            i++;
        }
    }
    return cleared;
}

void clear_grid(void) { memset(GRID, 0, sizeof(GRID)); }
