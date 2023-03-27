#include <stdint.h>

typedef enum TetriminoType { I, J, L, O, S, T, Z } TetriminoType;

typedef enum TetriminoState {
    First,
    Second,
    Third,
    Fourth,
} TetriminoState;

typedef enum Direction { Left, Right, Down } Direction;

typedef enum MoveResult {
    // Moved successfully
    Success,
    // Couldn't move horizontally
    HitSide,
    // Failed vertically and should be locked into GRID
    HitBottom,
} MoveResult;

typedef struct Tetrimino {
    enum TetriminoType type;
    // index into the current coordinates/rotation
    enum TetriminoState state;
    // pointer to list of states
    const uint8_t (*states)[4][4][4];
    // current position in grid, x is the column and y is the row
    int x, y;
} Tetrimino;

// Makes a new Tetrimino in the default position.
Tetrimino make_tetrimino(TetriminoType type);

TetriminoState next_state(TetriminoState state);

// Makes a random Tetrimino
Tetrimino random_tetrimino(void);
