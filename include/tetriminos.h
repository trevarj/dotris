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
    FailedH,
    // Failed vertically and should be locked into GRID
    FailedV,
} MoveResult;

typedef struct Tetrimino {
    enum TetriminoType type;
    // index into the current coordinates/rotation
    enum TetriminoState state;
    // pointer to list of states
    const char (*states)[4][4][4];
    // current position in grid, x is the column and y is the row
    int x, y;
} Tetrimino;

// Makes a new Tetrimino in the default position.
Tetrimino make_tetrimino(TetriminoType type);

// Makes a random Tetrimino
Tetrimino random_tetrimino(void);

// Move piece left, right or down
MoveResult move_tetrimino(Tetrimino *t, Direction d);

// Alters the state of the tetrimino if possible. Rotates clockwise.
void rotate_tetrimino(Tetrimino *t);

// Hard drop and return rows covered
int drop_tetrimino(Tetrimino *t);

// Locks Tetrimino into GRID
void write_to_grid(Tetrimino *t);

// Removes Tetrimino from GRID
void remove_from_grid(Tetrimino *t);
