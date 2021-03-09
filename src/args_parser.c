#include "args_parser.h"

int parse_args(cmd_args_t* args, int argc, char* argv[], char* envp[]) {
    args->recursive = true;
    args->verbose = false;
    args->verbose_on_modidy = true;
    printf("Ok!");
    return 0;
}
