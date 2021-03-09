#include "args_parser.h"
#include "logger.h"
#include <stdint.h>

int main(int argc, char* argv[], char* envp[]) {
    // cmd_args_t args;
    // parse_args(&args, argc, argv, envp);
    // printf("Args obtained:\nR - %d\nv  - %d\nc  - %d\n", args.recursive,
    //        args.verbose, args.verbose_on_modify);

    const char* log_file = get_log_filename(envp);
    if (log_file) printf("%s\n", log_file);
    return 0;
}
