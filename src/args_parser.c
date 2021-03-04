#include "args_parser.h"

extern int opterr;

int parse_args(cmd_args_t *args, int argc, char *argv[], char *envp[]) {
    memset(args, 0, sizeof(cmd_args_t));
    opterr = 0;

    int opt;
    while ((opt = getopt(argc, argv, "vcR")) != EOF) {
        switch (opt) {
            case 'v':
                args->verbose = true;
                args->verbose_on_modify = false;
                break;
            case 'c':
                args->verbose_on_modify = !args->verbose;
                break;
            case 'R':
                args->recursive = true;
                break;
            case '?':
                printf("Invalid flag!\n");
                break;
            default:
                break;
        }
    }

    return 0;
}
