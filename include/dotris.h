#ifndef DOTRIS_H
#define DOTRIS_H
#include <stdint.h>

#define CELL_WIDTH             2 // number of columns in braille char
#define CELL_HEIGHT            4 // number of rows in braille char

#define CELLS_X                5
#define CELLS_Y                5
#define GRID_BORDER_START_X    1
#define GRID_BORDER_START_Y    1
#define GRID_BORDER_END_X      (GRID_BORDER_START_X + CELLS_X + 1)
#define GRID_BORDER_END_Y      (GRID_BORDER_START_Y + CELLS_Y + 1)
#define GRID_BORDER_HORIZONTAL "─"
#define GRID_BORDER_VERTICAL   "│"
#define GRID_BORDER_TOP_LEFT   "┌"
#define GRID_BORDER_BOT_LEFT   "└"
#define GRID_BORDER_TOP_RIGHT  "┐"
#define GRID_BORDER_BOT_RIGHT  "┘"
#define GRID_START_X           (GRID_BORDER_START_X + 1)
#define GRID_START_Y           (GRID_BORDER_START_Y + 1)
#define GRID_END_X             (GRID_BORDER_END_X - 1)
#define GRID_END_Y             (GRID_BORDER_END_Y - 1)

#define GRID_HIDDEN_ROWS       2 // hidden rows to spawn pieces
#define GRID_WIDTH             (CELLS_X * CELL_WIDTH)
#define GRID_HEIGHT            ((CELLS_Y * CELL_HEIGHT) + GRID_HIDDEN_ROWS) // 22 rows high

typedef enum TetriminoType { I, J, L, O, S, T, Z, TETRIMINO_CNT } TetriminoType;

typedef enum TetriminoState {
    FIRST_STATE,
    SECOND_STATE,
    THIRD_STATE,
    FOURTH_STATE,
} TetriminoState;

typedef enum Direction { MOVE_LEFT, MOVE_RIGHT, MOVE_DOWN } TetriminoMoveDirection;

typedef enum MoveResult {
    // Moved successfully
    MOVE_SUCCESS,
    // Couldn't move horizontally
    MOVE_HIT_SIDE,
    // Failed vertically and should be locked into GRID
    MOVE_HIT_BOTTOM,
} TetriminoMoveResult;

typedef struct Tetrimino {
    enum TetriminoType  type;
    // index into the current coordinates/rotation
    enum TetriminoState state;
    // pointer to list of states
    const uint8_t (*states)[4][4][4];
    // current position in grid, x is the column and y is the row
    int x, y;
} Tetrimino;

Tetrimino           tetrimino_create(TetriminoType type);
TetriminoState      tetrimino_next_state(TetriminoState state);
Tetrimino           tetrimino_create_random(void);

void                grid_draw_border(void);
void                grid_draw(Tetrimino *t);
void                draw_held_tetrimino(int type);
void                draw_hud(int score, int level, int lines_left, int lines_per_level);

TetriminoMoveResult grid_move_tetrimino(Tetrimino *t, TetriminoMoveDirection d);
void                grid_rotate_tetrimino(Tetrimino *t);
int                 grid_drop_tetrimino(Tetrimino *t);
void                grid_write_tetrimino(Tetrimino *t);
void                grid_remove_tetrimino(Tetrimino *t);

int                 grid_clear_lines(void);
void                grid_clear_all(void);

typedef struct DotChar {
    const char *c1;
    const char *c2;
} DotChar;

DotChar dotmap_to_dotchar(const uint8_t (*dotmap)[4]);
DotChar char_to_dotchar(int a);

#endif // !DOTRIS_H
