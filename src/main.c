#include "args_parser.h"
#include "dirs.h"
#include "file_status.h"

int main(int argc, char *argv[], char *envp[]) {
    cmd_args_t args;
    parse_args(&args, argc, argv, envp);
    printf("Args obtained:\nR - %d\nv  - %d\nc  - %d\n", args.recursive, args.verbose, args.verbose_on_modify);

    // struct stat status;
    // get_status(argv[1], &status);
    // if (is_dir(&status)) recursive_change_mod(argv[1]);
    return 0;
}
