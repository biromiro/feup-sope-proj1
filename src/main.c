#include <stdint.h>

#include "args_parser.h"

int main(int argc, char *argv[], char *envp[]) {
    cmd_args_t args;
    parse_args(&args, argc, argv, envp);
    printf("Args obtained:\nR - %d\nv  - %d\nc  - %d\n", args.options.recursive, args.options.verbose, args.options.verbose_on_modify);
    printf("%o, %d, %d, %d\n", args.mode.permission_octal, args.mode.permission_types.type_u, args.mode.permission_types.type_g, args.mode.permission_types.type_o);

    printf("\nFiles: %d %d %d\n",
           (int)(args.files_end - args.files_end),
           (int)(args.files_start),
           (int)(args.files_end));
    for (size_t i = args.files_start; i < args.files_end; i++) {
        printf("- %s\n", argv[i]);
    }

    return 0;
}
