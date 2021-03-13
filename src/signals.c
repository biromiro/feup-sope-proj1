#include "../include/signals.h"

#include <ctype.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/aux.h"
#include "../include/process.h"

static bool waiting = false;

/**
 * @brief Verifies if process is waiting for user feedback
 *
 * @return process waiting for user feedback
 */
bool is_waiting() {
    return waiting;
}

void lock_process() {
    while (waiting) {
    }
}

bool is_root_process() {
    return getpid() == getpgrp();
}

void wait_for_children() {
}

/**
 * @brief Function that handles input when SIGINT received
 */
void handle_sig_int() {
}

/**
 * @brief Handler to be called when SIGINT is received
 */
void sig_int_process() {
    if (waiting) {
        printf("Process killed\n");
        exit(1);
    }
    waiting = true;

    pinfo_t *info = get_pinfo();

    printf("%d ; %s ; %d ; %d \n", getpid(), info->curr_file, 5, 4);
    if (getpid() == getpgrp()) {
        sleep(0.5);
        printf("Do you want to exit? (y/Y or n/N)\n");
        char c = get_clean_char();
        printf("CHAR OBTAINED: %c\n", c);
        switch (toupper(c)) {
            case 'Y':
                printf("SENT SIGNAL\n");
                killpg(getpgrp(), SIGINT);
                exit(1);
                break;
            case 'N':
            default:
                killpg(getpgrp(), SIGCONT);
                break;
        }
    }
}

/**
 * @brief Handler to be called when SIGCONT is received
 * SIGCONT in the program context is received when the 
 *user wants to continue execution after SIGINT
 */
void sig_cont_process() {
    if (waiting) {
        printf("Continue process\n");
        waiting = false;
    }
}

int setup_handlers() {
    struct sigaction new, old;
    sigset_t smask;

    if (sigemptyset(&smask) == -1) {
        perror("mask");
        return ERR_SIGEMPTYMASK;
    }

    new.sa_handler = sig_int_process;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGINT, &new, &old) == -1) {
        perror("sigaction");
        return ERR_SIGACTION;
    }

    if (sigemptyset(&smask) == -1) {
        perror("mask");
        return ERR_SIGEMPTYMASK;
    }

    new.sa_handler = sig_cont_process;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGCONT, &new, &old) == -1) {
        perror("sigaction");
        return ERR_SIGACTION;
    }

    return 0;
}
