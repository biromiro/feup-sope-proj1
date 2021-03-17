#include "../include/logger.h"

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../include/error/exit_codes.h"
#include "../include/process.h"

#define LOG_ENV_VAR "LOG_FILENAME"

typedef struct log_info {
    clock_t begin;
    bool logging;
    bool init;
    int file_descriptor;
} log_info_t;

static log_info_t log_info = {0};

static const char* const event_to_string[] = {
    "PROC_CREAT", "PROC_EXIT", "SIGNAL_RECV", "SIGNAL_SENT", "FILE_MODF"};

void init_log_info() {
    log_info.begin = clock();
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
        perror("log print");
        return errno;
    }

    return 0;
}

int write_log(enum Event event, const char* info) {
    if (!log_info.logging) return 0;

    int pid = getpid();
    int instant = clock() - log_info.begin;

    int err = dprintf(log_info.file_descriptor, "%d ; %d ; %s ; %s\n", instant,
                      pid, event_to_string[event], info);

    if (err < 0) {
        perror("log print");
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
            perror("open/create log (creat)");
            return errno;
        }
    } else {
        if ((log_info.file_descriptor =
                 open(path_name,
                      O_APPEND | O_WRONLY)) == -1) {
            printf("%s\n", path_name);
            perror("open/create log (open)");
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
        perror("closing log");
        return errno;
    }

    log_info.logging = false;
    return 0;
}
