#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "../include/args_parser.h"
#include "../include/dirs.h"
#include "../include/error/exit_codes.h"
#include "../include/file_status.h"
#include "../include/logger.h"
#include "../include/permission_caller.h"
#include "../include/process.h"
#include "../include/signals.h"

void cleanup(void) { 
    close_log(); 
}

int main(int argc, char* argv[], char* envp[]) {
    setup_pinfo();

    if (setup_handlers()) {
        fprintf(stderr, "Error setting up sig handlers");
        exit(ERR_SIGNAL_SETUP);
    }

    init_log_info();
    atexit(cleanup);

    int err;

    err = open_log();
    switch (err) {
        case 0:
        case NO_FILE_GIVEN:
            break;
        default:
            return err;
    }

    write_process_create_log(argc, argv);

    // struct stat status;
    // get_status(argv[1], &status);
    // if (is_dir(&status)) recursive_change_mod(argv[1]);

    cmd_args_t args;
    parse_args(&args, argc, argv, envp);
    // printf("Args obtained:\nR - %d\nv  - %d\nc  - %d\n", args.options.recursive,
    //        args.options.verbose, args.options.verbose_on_modify);
    // printf("%o, %d, %d, %d\n", args.mode.permission_octal,
    //        args.mode.permission_types.type_u, args.mode.permission_types.type_g,
    //        args.mode.permission_types.type_o);

    // printf("\nFiles: %lu %zu %zu\n", (args.files_end - args.files_start),
    //        (args.files_start), (args.files_end));
    handle_change_mods(&args, argv, envp);
    char out[255];
        int w_status;
        pid_t pid;

    while(true) {
        

        if((pid = wait(&w_status)) == -1 && errno == ECHILD) break;
        printf("exit here pid %d\n", pid);
        if(pid == -1) continue;

        
        snprintf(out, sizeof(out), "%d", WEXITSTATUS(w_status));
        write_log(PROC_EXIT, out);
    }

    return 0;
}
