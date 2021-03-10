#include "logger.h"
#include "error/exit_codes.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static log_info_t log_info;

void init_log_info() {
    log_info.begin = clock();
    log_info.file_descriptor = 0;
    log_info.logging = false;
}

bool is_logging() { return log_info.logging; }

int write_log_format(const char* format, ...) {
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

int write_log(int pid, const char* action_event, const char* info) {
    int err = dprintf(log_info.file_descriptor, "%ld ; %d ; %s ; %s\n", clock(),
                      pid, action_event, info);

    if (err < 0) {
        perror("log print");
        return errno;
    }

    return 0;
}

int open_log() {
    const char* path_name = getenv(LOG_ENV_VAR);
    if (!path_name || strlen(path_name) == 0) return NO_FILE_GIVEN;

    if ((log_info.file_descriptor =
             creat(path_name, S_IRWXU | S_IRWXG | S_IRWXO)) == -1) {
        perror("open/create log");
        return errno;
    }

    log_info.logging = true;
    return 0;
}

int close_log() {
    int err = close(log_info.file_descriptor);
    if (err) {
        perror("closing log");
        return errno;
    }

    log_info.logging = false;
    return 0;
}
