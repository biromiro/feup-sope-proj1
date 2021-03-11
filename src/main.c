#include "../include/logger.h"
#include "args_parser.h"
#include "error/exit_codes.h"
#include "dirs.h"
#include "file_status.h"
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
    
    // struct stat status;
    // get_status(argv[1], &status);
    // if (is_dir(&status)) recursive_change_mod(argv[1]);
    
    cmd_args_t args;
    parse_args(&args, argc, argv, envp);
    printf("Args obtained:\nR - %d\nv  - %d\nc  - %d\n", args.options.recursive, args.options.verbose, args.options.verbose_on_modify);
    printf("%o, %d, %d, %d\n", args.mode.permission_octal, args.mode.permission_types.type_u, args.mode.permission_types.type_g, args.mode.permission_types.type_o);

    printf("\nFiles: %d %d %d\n",
           (int)(args.files_end - args.files_start),
           (int)(args.files_start),
           (int)(args.files_end));
    for (size_t i = args.files_start; i < args.files_end; i++) {
        printf("- %s\n", argv[i]);
    }

    if (close_log()) {
        return errno;
    }
    
    return 0;
}
