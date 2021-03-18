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
#include "../include/logger.h"
#include "../include/process.h"

extern int errno;
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
void sig_int_process(int signo) {
    errno = 0;

    write_signal_recv_log(signo);

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
                write_signal_send_group_log(getpgrp(), SIGINT);
                exit(1);
                break;
            default:
                killpg(getpgrp(), SIGCONT);
                write_signal_send_group_log(getpgrp(), SIGCONT);
                break;
        }
    } else {
        kill(get_super_process(), SIGUSR1);
        write_signal_send_process_log(get_super_process(), SIGUSR1);
    }
}

void log_handler(int signo) {
    write_signal_recv_log(signo);
    if (signal(signo, SIG_DFL) == NULL) {
        perror("signal");
        return;
    }
    raise(signo);
}

int setup_log_signals() {
    struct sigaction new = {0}, old = {0};
    sigset_t smask;
    for (size_t i = 1; i <= 63; i++) {
        if (NO_OVERRIDE_SIG(i)) {
            if (sigemptyset(&smask) == -1) {
                perror("mask");
                return errno;
            }

            new.sa_handler = log_handler;
            new.sa_mask = smask;
            new.sa_flags = 0;
            if (sigaction(i, &new, &old) == -1) {
                printf("SIG: %lu\n", i);
                perror("sigaction");
                return errno;
            }
        }
    }
    return 0;
}

/**
 * @brief Handler to be called when SIGCONT is received
 * SIGCONT in the program context is received when the 
 *user wants to continue execution after SIGINT
 */
void sig_cont_process(int signo) {
    errno = 0;
    write_signal_recv_log(signo);
    if (waiting) {
        // printf("Continue process\n");
        waiting = false;
    }
}

/**
 * @brief Handler to be called when children sends SIGUSR1
 * This signal indicates that a specific children printed its log after SIGINT
 */
void sig_recv_children(int signo) {
    errno = 0;
    write_signal_recv_log(signo);
    last_recv = time(NULL);
}

int setup_handlers() {
    struct sigaction new = {0}, old= {0};
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

    if (sigemptyset(&smask) == -1) {
        perror("mask");
        return ERR_SIGEMPTYMASK;
    }

    new.sa_handler = sig_recv_children;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGUSR1, &new, &old) == -1) {
        perror("sigaction");
        return ERR_SIGACTION;
    }

    return setup_log_signals();
}
