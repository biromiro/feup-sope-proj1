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

int write_log(int file_descriptor, const char * format, ...) {
    va_list valist;

    va_start(valist, format);

    vdprintf(file_descriptor, format, valist);

    va_end(valist);

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
