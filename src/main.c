#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#include "../include/args_parser.h"
#include "../include/dirs.h"
#include "../include/aux.h"
#include "../include/error/exit_codes.h"
#include "../include/file_status.h"
#include "../include/logger.h"
#include "../include/permission_caller.h"
#include "../include/process.h"
#include "../include/signals.h"

void cleanup(void) {
    if (!is_silent_exit()) {
        char out[255];
        int code = get_exit_code();

        snprintf(out, sizeof(out), "%d", code);
        update_pid_pinfo(getpid());
        write_log(PROC_EXIT, out);
    }

    close_log();
}

int main(int argc, char* argv[], char* envp[]) {
    setup_pinfo();
    atexit(cleanup);

    if (setup_handlers()) {
        fprintf(stderr, "Error setting up sig handlers\n");
        set_and_exit(ERR_SIGNAL_SETUP);
    }

    init_log_info();

    int err;

    err = open_log();
    switch (err) {
        case 0:
        case NO_FILE_GIVEN:
            break;
        default:
            return err;
    }
    if (is_root_process()) {  // special case for first process creation log
        write_process_create_log(argc, argv);
    }

    // struct stat status;
    // get_status(argv[1], &status);
    // if (is_dir(&status)) recursive_change_mod(argv[1]);

    cmd_args_t args;
    parse_args(&args, argc, argv, envp);
    // printf("Args obtained:\nR - %d\nv  - %d\nc  - %d\n",
    // args.options.recursive,
    //        args.options.verbose, args.options.verbose_on_modify);
    // printf("%o, %d, %d, %d\n", args.mode.permission_octal,
    //        args.mode.permission_types.type_u,
    //        args.mode.permission_types.type_g,
    //        args.mode.permission_types.type_o);

    // printf("\nFiles: %lu %zu %zu\n", (args.files_end - args.files_start),
    //        (args.files_start), (args.files_end));
    handle_change_mods(&args, argv, envp);

    set_and_exit(0);
}
