#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct cmd_args {
    bool verbose;
    bool verbose_on_modidy;
    bool recursive;
    // Add modes and paths maybe
} cmd_args_t;

int parse_args(cmd_args_t* args, int argc, char* argv[], char* envp[]);
