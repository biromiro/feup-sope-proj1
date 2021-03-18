#include "../include/aux.h"

#include <stdio.h>
#include <stdlib.h>

int int_to_string(int num, char* string, int buffer_size) {
    int cur = 0;

    int aux = num;

    while (aux > 0) {
        cur++;
        aux /= 10;
    }

    if (cur >= buffer_size) cur = buffer_size - 1;
    string[cur] = '\0';
    int size = cur;
    cur--;

    while (num > 0 && cur >= 0) {
        string[cur] = num % 10 + '0';
        num /= 10;

        cur--;
    }

    return size;
}

void read_curr_time_ms(clock_ms_t* clock_val) {
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    *clock_val = tp.tv_sec * 1000 + (tp.tv_nsec / 1000000);
}

int get_clean_char() {
    char tmp;
    while (((tmp = getchar()) == EOF) || (tmp == '\n')) {
    }
    return tmp;
}
