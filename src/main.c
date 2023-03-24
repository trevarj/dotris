#include <curses.h>
#include <grid.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KEY_SPACE ' '

extern char GRID[GRID_HEIGHT][GRID_WIDTH]; // defined in grid.c

void setup(void) {
    setlocale(LC_ALL, "");
    initscr();
    int x, y;
    getmaxyx(stdscr, y, x);
    if (x < GRID_BORDER_END_X + 2 || y < GRID_BORDER_END_Y + 1) {
        printf("Screen must be at least %d wide and %d high.\nYours is %d by %d.\n",
               GRID_BORDER_END_X + 2, GRID_BORDER_END_Y + 1, x, y);
        endwin();
        exit(EXIT_FAILURE);
    }
    raw();                // Line buffering disabled
    halfdelay(1);         // 1/10th of a second delay on getch
    curs_set(0);          // Hide the cursor
    keypad(stdscr, true); // We get F1, F2 etc..
    noecho();             // Don't echo() while we do getch
}

// BPS scoring system
// https://tetris.wiki/Tetris_(BPS)
int calculate_score(int cleared, int drop_rows) {
    int score = 0;
    switch (cleared) {
    case 1:
        score += 40;
        break;
    case 2:
        score += 100;
        break;
    case 3:
        score += 300;
        break;
    case 4:
        score += 1200;
        break;
    }
    if (cleared > 0) {
        score += drop_rows;
    }
    return score;
}

// Increase drop speed by 25% every 10 lines cleared
void level_freq(double *freq, int cleared) {
    if (cleared && cleared % 10 == 0) {
        *freq *= .75;
        memset(GRID, 0, sizeof(GRID[0][0]) * GRID_WIDTH * GRID_HEIGHT);
    }
}

int main(void) {
    setup();
    draw_border();
    bool quit = false;
    int input;
    time_t ticker;
    time(&ticker);
    Tetrimino t = random_tetrimino();
    int total_cleared = 0;
    double tick_freq = 1.5;
    int score = 0;
    while (!quit) {
        MoveResult move_res = Success;
        int drop_rows = 0;
        time_t now = time(0);
        if (difftime(now, ticker) >= tick_freq) {
            ticker = now;
            move_res = move_tetrimino(&t, Down);
        }
        if (move_res == FailedV && t.y <= 0) {
            // GAME OVER!
            printf("Game Over! ");
            break;
        }
        input = getch();
        switch (input) {
        case KEY_LEFT:
            move_tetrimino(&t, Left);
            break;
        case KEY_RIGHT:
            move_tetrimino(&t, Right);
            break;
        case KEY_DOWN:
            move_tetrimino(&t, Down);
            break;
        case KEY_UP:
            rotate_tetrimino(&t);
            break;
        case KEY_SPACE:
            drop_rows = drop_tetrimino(&t);
            move_res = FailedV;
            break;
        case 'q':
            quit = true;
            break;
        }

        if (move_res == FailedV) {
            // lock piece
            write_to_grid(&t);
            // check and clear lines
            int cleared = clear_lines();
            if (cleared) {
                score += calculate_score(cleared, drop_rows);
                total_cleared += cleared;
                level_freq(&tick_freq, total_cleared);
            }
            // next piece
            t = random_tetrimino();
        }

        draw_grid(&t);
    }
    endwin();

    printf("You scored %d.\n", score);
    return 0;
}
