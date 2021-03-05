#pragma once

#include <errno.h>
#include <fcntl.h> /* Definition of AT_* constants */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define ERROR_FLAG -1
#define READ_VAL 4
#define WRITE_VAL 2
#define EXEC_VAL 1

typedef struct cmd_args {
    bool verbose;
    bool verbose_on_modidy;
    bool recursive;
    // Add modes and paths maybe
} cmd_args_t;

int parse_args(cmd_args_t *args, int argc, char *argv[], char *envp[]);

mode_t parse_mode(char *mode);

mode_t parse_octal_mode(char *mode);

mode_t parse_text_mode(char *mode);

char parse_user_type_perms(char *user_perms);

char get_sum_perms(char *perms);