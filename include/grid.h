#include <tetriminos.h>

#define CELL_WIDTH 2  // number of columns in braille char
#define CELL_HEIGHT 4 // number of rows in braille char

#define CELLS_X 5
#define CELLS_Y 5
#define GRID_BORDER_START_X 1
#define GRID_BORDER_START_Y 1
#define GRID_BORDER_END_X (GRID_BORDER_START_X + CELLS_X + 1)
#define GRID_BORDER_END_Y (GRID_BORDER_START_Y + CELLS_Y + 1)
#define GRID_BORDER_HORIZONTAL "─"
#define GRID_BORDER_VERTICAL "│"
#define GRID_BORDER_TOP_LEFT "┌"
#define GRID_BORDER_BOT_LEFT "└"
#define GRID_BORDER_TOP_RIGHT "┐"
#define GRID_BORDER_BOT_RIGHT "┘"
#define GRID_START_X (GRID_BORDER_START_X + 1)
#define GRID_START_Y (GRID_BORDER_START_Y + 1)
#define GRID_END_X (GRID_BORDER_END_X - 1)
#define GRID_END_Y (GRID_BORDER_END_Y - 1)

#define GRID_HIDDEN_ROWS 2 // hidden rows to spawn pieces
#define GRID_WIDTH (CELLS_X * CELL_WIDTH)
#define GRID_HEIGHT ((CELLS_Y * CELL_HEIGHT) + GRID_HIDDEN_ROWS) // 22 rows high

void draw_border(void);
void draw_grid(Tetrimino *t);

// Move piece left, right or down
MoveResult move_tetrimino(Tetrimino *t, Direction d);
// Alters the state of the tetrimino if possible. Rotates clockwise.
void rotate_tetrimino(Tetrimino *t);
// Hard drop and return rows covered
int drop_tetrimino(Tetrimino *t);

void write_to_grid(Tetrimino *t);
void remove_from_grid(Tetrimino *t);

int clear_lines(void);
void clear_grid(void);
