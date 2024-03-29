#include <stdio.h>
#include <stdlib.h>

#include <curses.h>
#include <locale.h>
#include <string.h>
#include <time.h>

#include "dotris.h"

#define KEY_SPACE          ' '
#define KEY_C              'c'
#define KEY_Q              'q'
#define LINES_PER_LEVEL    10
#define STARTING_FREQ_SECS 1500
#define NONE_HELD          -1
#define RESTORED_HELD      -2

void setup(bool *draw_hud) {
    setlocale(LC_ALL, "");
    initscr();
    int x, y;
    getmaxyx(stdscr, y, x);
    if (x < GRID_BORDER_END_X + 2 || y < GRID_BORDER_END_Y + 1) {
        printf("Screen must be at least %d wide and %d high.\nYours is %d by %d.\n",
               GRID_BORDER_END_X + 2, GRID_BORDER_END_Y + 1, x, y);
        endwin();
        exit(EXIT_FAILURE);
    } else if (x < GRID_BORDER_END_X + 6) {
        *draw_hud = false;
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

void level_up(int *level, int64_t *freq) {
    *freq *= .6;
    (*level)++;
}

void level_down(int *level, int64_t *freq) {
    *freq /= .6;
    (*level)--;
}

void change_level(int direction, int *level, int64_t *freq, int *lines_left, int *score,
                  bool can_draw_hud) {
    if (direction)
        level_up(level, freq);
    else
        level_down(level, freq);
    *lines_left = LINES_PER_LEVEL;
    grid_clear_all();
    if (can_draw_hud)
        draw_hud(*score, *level, *lines_left, LINES_PER_LEVEL);
}

int main(int argc, char *argv[]) {
    bool    quit = false, can_draw_hud = true;
    int64_t tick_freq = STARTING_FREQ_SECS, ticker = 0;
    int held_piece = NONE_HELD, input, lines_left = LINES_PER_LEVEL, score = 0, total_cleared = 0,
        level = 1;

    if (argc > 1 && strcmp(argv[1], "-v") == 0) {
        printf("dotris %s\n", VERSION);
        exit(0);
    }

    setup(&can_draw_hud);
    grid_draw_border();
    if (can_draw_hud)
        draw_hud(score, level, lines_left, LINES_PER_LEVEL);

    Tetrimino t = tetrimino_create_random();

    while (!quit) {
        TetriminoMoveResult move_res  = MOVE_SUCCESS;
        int                 drop_rows = 0;
        struct timespec     now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        int64_t now_ms = now.tv_sec * INT64_C(1000) + now.tv_nsec / 1000000;

        if (now_ms - ticker >= tick_freq) {
            ticker   = now_ms;
            move_res = grid_move_tetrimino(&t, MOVE_DOWN);
        }
        if (move_res == MOVE_HIT_BOTTOM && t.y <= 0) {
            printf("Game Over! ");
            break;
        }

        input = getch();
        switch (input) {
        case KEY_LEFT:
            grid_move_tetrimino(&t, MOVE_LEFT);
            break;
        case KEY_RIGHT:
            grid_move_tetrimino(&t, MOVE_RIGHT);
            break;
        case KEY_DOWN:
            grid_move_tetrimino(&t, MOVE_DOWN);
            break;
        case KEY_UP:
            grid_rotate_tetrimino(&t);
            break;
        case KEY_SPACE:
            drop_rows = grid_drop_tetrimino(&t);
            move_res  = MOVE_HIT_BOTTOM;
            break;
        case KEY_C:
            if (held_piece >= 0) {
                t          = tetrimino_create((TetriminoType)held_piece);
                held_piece = RESTORED_HELD;
            } else if (held_piece == NONE_HELD) {
                held_piece = (int)t.type;
                t          = tetrimino_create_random();
            }
            break;
        case KEY_PPAGE:
            if (level < 100)
                change_level(1, &level, &tick_freq, &lines_left, &score, can_draw_hud);
            break;
        case KEY_NPAGE:
            if (level > 1) {
                change_level(0, &level, &tick_freq, &lines_left, &score, can_draw_hud);
            }
            break;
        case KEY_Q:
            quit = true;
            break;
        }

        if (move_res == MOVE_HIT_BOTTOM) {
            grid_write_tetrimino(&t);
            int cleared = grid_clear_lines();
            if (cleared) {
                total_cleared += cleared;
                lines_left    -= cleared;
                score         += calculate_points(cleared, drop_rows, level);
                if (lines_left <= 0)
                    change_level(1, &level, &tick_freq, &lines_left, &score, can_draw_hud);
            }
            if (held_piece == RESTORED_HELD)
                held_piece = NONE_HELD;
            t = tetrimino_create_random();
        }

        grid_draw(&t);
        draw_held_tetrimino(held_piece);
    }
    endwin();

    printf("You scored %d and cleared %d lines!\n", score, total_cleared);
    return 0;
}
