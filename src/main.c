#include "args_parser.h"
#include "logger.h"
#include <fcntl.h>
#include <stdint.h>

int main(int argc, char* argv[], char* envp[]) {
    // cmd_args_t args;
    // parse_args(&args, argc, argv, envp);
    // printf("Args obtained:\nR - %d\nv  - %d\nc  - %d\n", args.recursive,
    //        args.verbose, args.verbose_on_modify);
    int fd;
    open_log(&fd);
    return 0;
}
