#include "../include/logger.h"
#include "args_parser.h"
#include "error/exit_codes.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc, char* argv[], char* envp[]) {
    // cmd_args_t args;
    // parse_args(&args, argc, argv, envp);
    // printf("Args obtained:\nR - %d\nv  - %d\nc  - %d\n", args.recursive,
    //        args.verbose, args.verbose_on_modify);
    init_log_info();

    int err;

    err = open_log();
    switch (err) {
        case 0:
            printf("i am logging\n");

            break;
        case NO_FILE_GIVEN:
            printf("no logging file given, logging disabled\n");

            break;
        default:
            return err;
    }

    printf("is logging? %d\n", is_logging());

    write_log_format("%s\n", "this is a test");
    write_log(PROC_CREAT, "this is a dummy event.");
    write_log(PROC_CREAT, "this is a dummy event.");

    write_log(PROC_CREAT, "this is a dummy event.");
    write_log(PROC_CREAT, "this is a dummy event.");

    for (int i = 0; i < INT32_MAX; i++) {
    }
    write_log(PROC_CREAT, "this is a dummy event.");
    write_log(PROC_EXIT, "this is a dummy event.");

    if (close_log()) {
        return errno;
    }

    return 0;
}
