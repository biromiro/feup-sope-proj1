#include "../include/logger.h"

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../include/aux.h"
#include "../include/dirs.h"
#include "../include/error/exit_codes.h"
#include "../include/process.h"
#include "../include/signals.h"

#define LOG_ENV_VAR "LOG_FILENAME"

typedef struct log_info {
    clock_ms_t begin;
    bool logging;
    bool init;
    int file_descriptor;
} log_info_t;

static log_info_t log_info = {0};

static const char* const event_to_string[] = {
    "PROC_CREAT", "PROC_EXIT", "SIGNAL_RECV", "SIGNAL_SENT", "FILE_MODF"};

/**
 * @brief Setups envp to send the initial timer
 */
void setup_envp(clock_ms_t start_time) {
    char env_buf[128];

    snprintf(env_buf, sizeof(env_buf), "%lu", start_time);
    setenv("PROC_START_TIME_MS", env_buf, 1);
}

void init_log_info() {
    clock_ms_t start_time = 0;
    char* env_start;
    if (is_root_process()) {
        read_curr_time_ms(&start_time);
        setup_envp(start_time);
    } else {
        env_start = getenv("PROC_START_TIME_MS");
        if (env_start != NULL) start_time = atol(env_start);
    }
    log_info.begin = start_time;
    log_info.file_descriptor = 0;
    log_info.logging = false;
    log_info.init = true;
}

bool is_logging() { return log_info.logging; }

int write_log_format(const char* format, ...) {
    if (!log_info.logging) return 0;

    va_list valist;

    va_start(valist, format);

    int err = vdprintf(log_info.file_descriptor, format, valist);

    va_end(valist);

    if (err < 0) {
        return errno;
    }

    return 0;
}

int write_log(enum Event event, const char* info) {
    if (!log_info.logging) return 0;

    int pid = event == PROC_EXIT ? get_pinfo()->child_pid : getpid();

    clock_ms_t current_time_ms = 0;
    read_curr_time_ms(&current_time_ms);
    clock_ms_t instant = current_time_ms - log_info.begin;

    char out[128] = "";
    snprintf(out, sizeof(out), "%lu ; %d ; %s ; %s\n", instant, pid,
             event_to_string[event], info);

    struct flock lock;
    memset(&lock, 0, sizeof(lock));

    lock.l_type = F_WRLCK;

    while (fcntl(log_info.file_descriptor, F_SETLK, &lock) == -1 &&
           (errno == EACCES || errno == EAGAIN)) {
    }

    lseek(log_info.file_descriptor, 0, SEEK_END);
    int err;
    while ((err = write(log_info.file_descriptor, out, strlen(out))) == -1 &&
           errno == EINTR)
        ;  // if interrupted by sig handler try again.

    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_UNLCK;
    if (fcntl(log_info.file_descriptor, F_SETLK, &lock)) {
        return errno;
    }

    if (err < 0) {
        return errno;
    }

    return 0;
}

int open_log() {
    if (!log_info.init) return NO_FILE_GIVEN;

    const char* path_name = getenv(LOG_ENV_VAR);
    if (!path_name || strlen(path_name) == 0) return NO_FILE_GIVEN;

    if (is_root_process()) {
        if ((log_info.file_descriptor =
                 creat(path_name, S_IRWXU | S_IRWXG | S_IRWXO)) == -1) {
            return errno;
        }
    } else {
        if ((log_info.file_descriptor = open(path_name, O_APPEND | O_WRONLY)) ==
            -1) {
            printf("%s\n", path_name);
            return errno;
        }
    }

    log_info.logging = true;
    return 0;
}

int close_log() {
    if (!log_info.logging) return 0;

    int err = close(log_info.file_descriptor);
    if (err) {
        return errno;
    }

    log_info.logging = false;
    return 0;
}

int write_permission_log(const char* pathname, mode_t current_permission,
                         mode_t new_permission) {
    if (current_permission == new_permission) {
        return 0;
    }

    // 8 + spaces and ':'
    const size_t kSize = strlen(pathname) + 15;

    char info[kSize];
    char curr_perm_str[5];
    char new_perm_str[5];

    octal_to_string(current_permission, curr_perm_str);
    octal_to_string(new_permission, new_perm_str);

    snprintf(info, kSize, "%s : %s : %s", pathname, curr_perm_str,
             new_perm_str);
    return write_log(FILE_MODF, info);
}

int write_process_create_log(int argc, char* argv[]) {
    size_t size = 0;

    for (size_t i = 0; i < argc; i++) size += strlen(argv[i]) + 1;

    char* cmd_line = malloc(size);
    char* begin = cmd_line;
    for (size_t i = 0; i < argc; i++) {
        begin += snprintf(begin, size, i > 0 ? " %s" : "%s", argv[i]);
    }

    int res = write_log(PROC_CREAT, cmd_line);

    free(cmd_line);

    return res;
}

int write_signal_recv_log(int signo) {
    char sig[4];
    snprintf(sig, sizeof(sig) - 1, "%d", signo);
    return write_log(SIGNAL_RECV, sig);
}

int write_signal_send_group_log(int pid, int signo) {
    char sig[50];
    snprintf(sig, sizeof(sig) - 1, "%d : %d (group)", signo, pid);
    return write_log(SIGNAL_SENT, sig);
}

int write_signal_send_process_log(int pid, int signo) {
    char sig[50];
    snprintf(sig, sizeof(sig) - 1, "%d : %d", signo, pid);
    return write_log(SIGNAL_SENT, sig);
}
