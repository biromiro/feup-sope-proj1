#include "args_parser.h"
#include "error/exit_codes.h"
#include "logger.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char* argv[], char* envp[]) {
    // cmd_args_t args;
    // parse_args(&args, argc, argv, envp);
    // printf("Args obtained:\nR - %d\nv  - %d\nc  - %d\n", args.recursive,
    //        args.verbose, args.verbose_on_modify);
    int fd, err;
    bool logging = false;

    err = open_log(&fd);
    switch (err) {
        case 0:
            logging = true;
            printf("i am logging\n");

            break;
        case NO_FILE_GIVEN:
            printf("no logging file give, logging disabled\n");

            break;
        default:
            return err;
    }

    return 0;
}
