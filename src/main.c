#include <stdint.h>

#include "args_parser.h"

int main(int argc, char *argv[], char *envp[]) {
    cmd_args_t args;
    parse_args(&args, argc, argv, envp);
    printf("Args obtained:\nR - %d\nv  - %d\nc  - %d\n", args.recursive, args.verbose, args.verbose_on_modify);
    return 0;
}
