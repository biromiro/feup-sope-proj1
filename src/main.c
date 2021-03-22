#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/args_parser.h"
#include "../include/aux.h"
#include "../include/dirs.h"
#include "../include/error/exit_codes.h"
#include "../include/file_status.h"
#include "../include/logger.h"
#include "../include/permission_caller.h"
#include "../include/process.h"
#include "../include/signals.h"

void cleanup(void) {
    unsetup_handlers();

    if (!is_silent_exit()) {
        char out[255] = "";
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

    int err = 0;

    err = open_log();
    switch (err) {
        case 0:
        case NO_FILE_GIVEN:
            break;
        default:
            return err;
    }
    err = 0;
    if (is_root_process()) {  // special case for first process creation log
        write_process_create_log(argc, argv);
    }
    lock_process();

    cmd_args_t args;
    parse_args(&args, argc, argv, envp);
    err = handle_change_mods(&args, argv, envp);

    lock_process();
    set_and_exit(err);
}
