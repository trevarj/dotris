#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <check.h>

#include "../include/grid.h"
#include "../include/tetriminos.h"

START_TEST(test_char_to_dots) {
    DotChar a;
    char *input = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *expected = "⣏⡇⣹⡀⣩⡃⣹⡇⠓⡇⣛⡅⣯⡅⡩⠃⣿⡇⣛⡇⡯⡇⣟⡅⣏⡁⣏⡆⣿⡁⡟⠃⣯⡅⡗⡇⢸ ⣍⡇⡷⡁⣇⡀⡗⡇⡗⡇⣏⡇⡯⠇⣏⣇⡯⡃⣫⡅⢹⠁⣇⡇⢧⠇⣷⡇⡱⡁⢱⠁⣩⡃";
    int exp_len = strlen(expected);
    char output[exp_len + 1]; // +1 null byte
    int i, offset = 0;
    for (i = 0; i < (int)strlen(input); i++) {
        a = char_to_dotchar(input[i]);
        snprintf(output + offset, exp_len, "%s%s", a.c1, a.c2);
        offset += strlen(a.c1) + strlen(a.c2);
    }
    ck_assert_str_eq(expected, output);
    ck_assert_int_eq(exp_len, strlen(output));
}
END_TEST

START_TEST(test_tetriminos_to_dots) {
    char *expected = " ⡇⠠⢤⡤⠄⠰⠆⢀⡤⠠⡤⢤⡀";
    int exp_len = strlen(expected), offset = 0;
    char output[exp_len + 1 + 200];
    for (int t_type = I; t_type <= Z; t_type++) {
        Tetrimino t = make_tetrimino(t_type);
        DotChar dots = dotmap_to_dotchar(*t.states[0]);
        sprintf(output + offset, "%s%s", dots.c1, dots.c2);
        offset += strlen(dots.c1) + strlen(dots.c2);
    }
    ck_assert_str_eq(expected, output);
    ck_assert_int_eq(exp_len, strlen(output));
}
END_TEST

Suite *grid_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Grid");
    tc_core = tcase_create("Dots");

    tcase_add_test(tc_core, test_char_to_dots);
    tcase_add_test(tc_core, test_tetriminos_to_dots);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = grid_suite();
    sr = srunner_create(s);
    srunner_set_fork_status(sr, CK_NOFORK);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
