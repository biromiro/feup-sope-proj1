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
#include <unistd.h>

int write_log_format(int file_descriptor, const char* format, ...) {
    va_list valist;

    va_start(valist, format);

    int err = vdprintf(file_descriptor, format, valist);

    va_end(valist);

    if (err < 0) {
        perror("log print");
        return errno;
    }

    return 0;
}

int write_log(int file_descriptor, time_t instant, int pid,
              const char* action_event, const char* info) {
    int err = dprintf(file_descriptor, "%ld ; %d ; %s ; %s\n", instant, pid,
                      action_event, info);

    if (err < 0) {
        perror("log print");
        return errno;
    }

    return 0;
}

int open_log(int* file_descriptor) {
    const char* path_name = getenv(LOG_ENV_VAR);
    if (!path_name || strlen(path_name) == 0) return NO_FILE_GIVEN;

    if ((*file_descriptor = creat(path_name, S_IRWXU | S_IRWXG | S_IRWXO)) ==
        -1) {
        perror("open/create log");
        return errno;
    }

    return 0;
}

int close_log(int file_descriptor) {
    int err = close(file_descriptor);
    if (err) {
        perror("closing log");
        return errno;
    }

    return 0;
}