#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int open_log(int* file_descriptor) {
    const char* path_name = getenv(LOG_ENV_VAR);
    if (path_name) printf("%s\n", path_name);
    return 0;
}
