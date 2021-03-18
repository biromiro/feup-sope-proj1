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
#include "../include/logger.h"

extern int errno;
static bool waiting = false;
static time_t last_recv;

bool is_waiting() { return waiting; }

void lock_wait_process() {
    int wstat;
    while (1) {
        if (waiting) continue;
        if (wait(&wstat) > 0) break;
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
void handle_sig_int() {}

/**
 * @brief Handler to be called when SIGINT is received
 */
void sig_int_process(int signo) {
    errno = 0;

    write_signal_recv_log(signo);

    if (waiting) {
        char out[255];
        snprintf(out, sizeof(out), "%d", -SIGINT);
        update_pid_pinfo(getpid());
        write_log(PROC_EXIT, out);
        fflush(NULL);
        exit(1);
    }
    waiting = true;

    pinfo_t* info = get_pinfo();

    printf("%d ; %s ; %d ; %d \n", getpid(), info->curr_file, 5, 4);
    if (is_root_process()) {
        last_recv = time(NULL);

        wait_for_children();
    
        dprintf(STDERR_FILENO,"Do you want to exit? (y/Y or other to continue)\n");
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

void sig_chld() {
    int w_status;
    pid_t pid;
    pid = wait(&w_status);
    char out[255];

    if (pid > 0) {
        if(WIFSIGNALED(w_status) && WTERMSIG(w_status) != SIGINT) {
            snprintf(out, sizeof(out), "%d", -WTERMSIG(w_status));
            write_log(PROC_EXIT, out);
        } else if (!WIFSIGNALED(w_status)){
            snprintf(out, sizeof(out), "%d", WEXITSTATUS(w_status));
            write_log(PROC_EXIT, out);
        }
        fflush(NULL);
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

    if (sigemptyset(&smask) == -1) perror("mask");

    new.sa_handler = sig_recv_children;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGUSR1, &new, &old) == -1) perror("sigaction");

    new.sa_handler = sig_chld;
    new.sa_mask = smask;
    new.sa_flags = 0;

    if (sigaction(SIGCHLD, &new, &old) == -1) perror("sigaction");

    return 0;
}
