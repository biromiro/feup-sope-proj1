#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/args_parser.h"
#include "../include/dirs.h"
#include "../include/error/exit_codes.h"
#include "../include/file_status.h"
#include "../include/logger.h"
#include "../include/permission_caller.h"

void cleanup(void) { close_log(); }

int main(int argc, char* argv[], char* envp[]) {
    // cmd_args_t args;
    // parse_args(&args, argc, argv, envp);
    // printf("Args obtained:\nR - %d\nv  - %d\nc  - %d\n", args.recursive,
    //        args.verbose, args.verbose_on_modify);
    init_log_info();
    atexit(cleanup);

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

    // struct stat status;
    // get_status(argv[1], &status);
    // if (is_dir(&status)) recursive_change_mod(argv[1]);

    cmd_args_t args;
    parse_args(&args, argc, argv, envp);
    printf("Args obtained:\nR - %d\nv  - %d\nc  - %d\n", args.options.recursive,
           args.options.verbose, args.options.verbose_on_modify);
    printf("%o, %d, %d, %d\n", args.mode.permission_octal,
           args.mode.permission_types.type_u, args.mode.permission_types.type_g,
           args.mode.permission_types.type_o);

    printf("\nFiles: %lu %zu %zu\n", (args.files_end - args.files_start),
           (args.files_start), (args.files_end));
    for (size_t i = args.files_start; i < args.files_end; i++) {
        printf("- %s\n", argv[i]);
        //if(args.options.recursive) recursive_change_mod(argv[i], &args.mode);
        //else
        if (change_perms(argv[i], &args.mode) == 0)
            printf("Permissions changed");
        else
            printf("Permissions not changed!");
    }

    return 0;
}
