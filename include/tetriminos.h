#include <stdint.h>

typedef enum TetriminoType { I, J, L, O, S, T, Z } TetriminoType;

typedef enum TetriminoState {
    FIRST_STATE,
    SECOND_STATE,
    THIRD_STATE,
    FOURTH_STATE,
} TetriminoState;

typedef enum Direction { MOVE_LEFT, MOVE_RIGHT, MOVE_DOWN } Direction;

typedef enum MoveResult {
    // Moved successfully
    MOVE_SUCCESS,
    // Couldn't move horizontally
    MOVE_HIT_SIDE,
    // Failed vertically and should be locked into GRID
    MOVE_HIT_BOTTOM,
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
