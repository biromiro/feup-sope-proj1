#include <stdint.h>

#include "args_parser.h"

int main(int argc, char *argv[], char *envp[]) {
    printf("Main!\n");
    cmd_args_t args;
    parse_args(&args, argc, argv, envp);
    printf("Args obtained:\n  - %d\n  - %d\n  - %d\n", args.recursive, args.verbose, args.verbose_on_modidy);
    return 0;
}
