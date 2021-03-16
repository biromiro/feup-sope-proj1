#include "../include/signals.h"

#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "../include/aux.h"
#include "../include/process.h"

static bool waiting = false;
static time_t last_recv;

bool is_waiting() {
    return waiting;
}

void lock_wait_process() {
    int wstat;
    while (1) {
        if (waiting)
            continue;
        if (wait(&wstat) > 0)
            break;
    }
}

void lock_process() {
    while (waiting) {
    }
}

/**
 * @brief First process - the "oldest" parent
 * 
 * @return first process of the chain
 */
int get_super_process() {
    return getpgrp();
}

/**
 * Is the root process
 * 
 * @return true if root process
 */
bool is_root_process() {
    return getpid() == get_super_process();
}

/**
 * @brief Waits for children to print their info about the process after SIGKILL
 */
void wait_for_children() {
    while (time(NULL) <= last_recv) {
    }
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
    errno = 0;
    if (waiting) {
        exit(1);
    }
    waiting = true;

    print_proc_info();
    if (is_root_process()) {
        last_recv = time(NULL);

        wait_for_children();

        printf("Do you want to exit? (y/Y to exit or other to continue)\n");
        char c = get_clean_char();
        switch (toupper(c)) {
            case 'Y':
                killpg(getpgrp(), SIGINT);
                exit(1);
                break;
            default:
                killpg(getpgrp(), SIGCONT);
                break;
        }
    } else {
        kill(get_super_process(), SIGUSR1);
    }
}

/**
 * @brief Handler to be called when SIGCONT is received
 * SIGCONT in the program context is received when the 
 *user wants to continue execution after SIGINT
 */
void sig_cont_process() {
    errno = 0;
    if (waiting) {
        // printf("Continue process\n");
        waiting = false;
    }
}

/**
 * @brief Handler to be called when children sends SIGUSR1
 * This signal indicates that a specific children printed its log after SIGINT
 */
void sig_recv_children() {
    errno = 0;
    last_recv = time(NULL);
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

    if (sigemptyset(&smask) == -1)
        perror("mask");

    new.sa_handler = sig_recv_children;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGUSR1, &new, &old) == -1)
        perror("sigaction");

    return 0;
}
