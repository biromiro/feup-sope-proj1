#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error/parse_error.h"

typedef struct cmd_args {
    bool verbose;
    bool verbose_on_modify;
    bool recursive;
    mode_t mode;
    // Add modes and paths maybe
} cmd_args_t;

int parse_args(cmd_args_t *args, int argc, char *argv[], char *envp[]);
