#include <stdio.h>
#include <stdlib.h>

#include <curses.h>
#include <locale.h>
#include <string.h>
#include <time.h>

#include "grid.h"

#define KEY_SPACE ' '
#define KEY_C 'c'
#define KEY_Q 'q'
#define LINES_PER_LEVEL 1
#define STARTING_FREQ_SECS 1500

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
    time_t t;
    srand((unsigned)time(&t)); // seed the RNG
}

// Based on BPS scoring system
// https://tetris.wiki/Tetris_(BPS)
int calculate_points(int cleared, int drop_rows, int level) {
    int points = 0;
    switch (cleared) {
    case 1:
        points = 40;
        break;
    case 2:
        points = 100;
        break;
    case 3:
        points = 300;
        break;
    case 4:
        points = 1200;
        break;
    }
    points *= level;
    if (cleared > 0) {
        points += drop_rows;
    }
    return points;
}

// Increase drop speed by 40% every 10 lines cleared
void level_freq(double *freq, int *lines_left) {
    if (*lines_left <= 0) {
        *freq *= .6;
        clear_grid();
        *lines_left = LINES_PER_LEVEL;
    }
}

int main(void) {
    bool quit = false;
    double tick_freq = STARTING_FREQ_SECS;
    int held = -1;
    int input;
    int lines_left = LINES_PER_LEVEL;
    int score = 0;
    int total_cleared = 0;
    int64_t ticker = 0;

    setup();
    draw_border();

    Tetrimino t = random_tetrimino();

    while (!quit) {
        MoveResult move_res = MOVE_SUCCESS;
        int drop_rows = 0;
        struct timespec now;

        timespec_get(&now, TIME_UTC);
        int64_t now_ms = now.tv_sec * INT64_C(1000) + now.tv_nsec / 1000000;

        if (now_ms - ticker >= tick_freq) {
            ticker = now_ms;
            move_res = move_tetrimino(&t, MOVE_DOWN);
        }
        if (move_res == MOVE_HIT_BOTTOM && t.y <= 0) {
            printf("Game Over! ");
            break;
        }

        input = getch();
        switch (input) {
        case KEY_LEFT:
            move_tetrimino(&t, MOVE_LEFT);
            break;
        case KEY_RIGHT:
            move_tetrimino(&t, MOVE_RIGHT);
            break;
        case KEY_DOWN:
            move_tetrimino(&t, MOVE_DOWN);
            break;
        case KEY_UP:
            rotate_tetrimino(&t);
            break;
        case KEY_SPACE:
            drop_rows = drop_tetrimino(&t);
            move_res = MOVE_HIT_BOTTOM;
            break;
        case KEY_C:
            if (held != -1) {
                t = make_tetrimino((TetriminoType)held);
                held = -1;
            } else {
                held = (int)t.type;
                t = random_tetrimino();
            }
            break;
        case KEY_Q:
            quit = true;
            break;
        }

        if (move_res == MOVE_HIT_BOTTOM) {
            write_to_grid(&t);
            int cleared = clear_lines();
            if (cleared) {
                int level = total_cleared / LINES_PER_LEVEL;
                score += calculate_points(cleared, drop_rows, level);
                total_cleared += cleared;
                lines_left -= cleared;
                level_freq(&tick_freq, &lines_left);
            }
            t = random_tetrimino();
        }

        draw_grid(&t);
    }
    endwin();

    printf("You scored %d and cleared %d lines!\n", score, total_cleared);
    return 0;
}
