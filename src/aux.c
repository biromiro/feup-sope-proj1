#include "../include/aux.h"

#include <stdio.h>
#include <stdlib.h>

static int exit_code = 0;
static bool silent_exit = true;

int get_clean_char() {
    char c = getchar(), tmp;
    while (((tmp = getchar()) != EOF) && (tmp != '\n')) {
    }
    return c;
}

int get_exit_code() {
    return exit_code;
}

bool is_silent_exit() {
    return silent_exit;
}

void set_and_exit(int exit_c) {
    exit_code = exit_c;
    silent_exit = false;

    exit(exit_c);
}
