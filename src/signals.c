#include "../include/signals.h"

#define _GNU_SOURCE

#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/aux.h"
#include "../include/logger.h"
#include "../include/process.h"

static bool waiting = false;
static clock_ms_t last_recv;
static int pipes[2];

bool is_waiting() { return waiting; }

/**
 * @brief Waits for children to print their info about the process after SIGKILL
 */
void wait_for_children() {
    clock_ms_t time;

    do {
        read_curr_time_ms(&time);
        lock_process();
    } while (time <= last_recv);
}

void catch_int(int signo) {
    char out[4];
    int size = int_to_string(signo, out, sizeof(out));
    int err = 0, original_errno = errno;

    errno = 0;
    for (int i = 0; i <= size;) {
        err = write(pipes[1], out + i, 1);
        if (err == -1 && errno == EINTR) continue;

        i++;
    }
    errno = original_errno;
}

void catch_usr1(int signo) {
    read_curr_time_ms(&last_recv);
    last_recv += TIMEOUT;

    catch_int(signo);
}

void catch_sig_int(int signo) {
    if (waiting) {
        set_and_exit(-signo);
    }
    waiting = true;

    catch_int(signo);
}

/**
 * @brief Handler to be called when SIGINT is received
 */
void handle_sig_int(int signo) {
    errno = 0;

    write_signal_recv_log(signo);

    print_proc_info();
    if (is_root_process()) {
        waiting = false;
        read_curr_time_ms(&last_recv);
        last_recv += TIMEOUT;

        wait_for_children();
        waiting = true;

        dprintf(STDERR_FILENO,
                "Do you want to exit? (y/Y or other to continue)\n");
        char c = get_clean_char();
        switch (toupper(c)) {
            case 'Y':
                killpg(getpgrp(), SIGINT);
                write_signal_send_group_log(getpgrp(), SIGINT);
                set_and_exit(-SIGINT);
                break;
            default:
                waiting = false;
                killpg(getpgrp(), SIGCONT);
                write_signal_send_group_log(getpgrp(), SIGCONT);
                break;
        }
    } else {
        waiting = true;
        kill(get_super_process(), SIGUSR1);
        write_signal_send_process_log(get_super_process(), SIGUSR1);
    }
}

/**
 * @brief Handler to be called when SIGCONT is received
 * SIGCONT in the program context is received when the
 *user wants to continue execution after SIGINT
 */
void handle_sig_cont(int signo) {
    errno = 0;
    write_signal_recv_log(signo);

    waiting = false;
}

/**
 * @brief Handler to be called when children sends SIGUSR1
 * This signal indicates that a specific children printed its log after SIGINT
 */
void handle_sig_usr1(int signo) {
    errno = 0;
    write_signal_recv_log(signo);
}

/**
 * @brief Handler to be called when children sends SIGCHLD
 */
void handle_sig_child(int signo) {
    errno = 0;
    write_signal_recv_log(signo);
}

void log_handler(int signo) {
    write_signal_recv_log(signo);
    if (signal(signo, SIG_DFL) == NULL) {
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
                return errno;
            }

            new.sa_handler = catch_int;
            new.sa_mask = smask;
            new.sa_flags = 0;
            if (sigaction(i, &new, &old) == -1) {
                return errno;
            }
        }
    }
    return 0;
}

int setup_handlers() {
    pipe2(pipes, O_NONBLOCK);
    struct sigaction new = {0}, old = {0};
    sigset_t smask;

    if (sigemptyset(&smask) == -1) {
        return errno;
    }

    new.sa_handler = catch_usr1;
    new.sa_mask = smask;
    new.sa_flags = 0;
    if (sigaction(SIGUSR1, &new, &old) == -1) {
        return errno;
    }

    if (sigemptyset(&smask) == -1) {
        return errno;
    }

    new.sa_handler = catch_sig_int;
    new.sa_mask = smask;
    new.sa_flags = 0;
    if (sigaction(SIGINT, &new, &old) == -1) {
        return errno;
    }

    return setup_log_signals();
}

void unsetup_handlers() {
    close(pipes[0]);
    close(pipes[1]);
}

void reset_handlers() { pipe2(pipes, O_NONBLOCK); }

void lock_process() {
    char out[4];
    char* cur;
    int err, signo;
    bool blocked = false;

    while (!blocked || waiting) {
        blocked = false;
        cur = out;
        err = 0;
        signo = 0;

        while (cur < out + sizeof(out)) {
            errno = 0;
            err = read(pipes[0], cur, 1);

            if (err == -1) {
                if (errno == EINTR) continue;

                if (errno == EWOULDBLOCK) {
                    blocked = true;
                    break;
                }

                return;
            }

            if (*cur == 0) break;
            cur++;
        }

        if (cur == out) continue;

        signo = atoi(out);

        switch (signo) {
            case SIGINT:
                handle_sig_int(signo);
                break;
            case SIGCONT:
                handle_sig_cont(signo);
                break;
            case SIGUSR1:
                handle_sig_usr1(signo);
                break;
            case SIGCHLD:
                handle_sig_child(signo);
                break;
            default:
                log_handler(signo);
                break;
        }
    }

    errno = 0;
}
