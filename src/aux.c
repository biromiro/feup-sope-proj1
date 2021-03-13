#include "../include/aux.h"

#include <stdio.h>
#include <stdlib.h>

int get_clean_char() {
    char c = getchar(), tmp;
    while (((tmp = getchar()) != EOF) && (tmp != '\n')) {
    }
    return c;
}
