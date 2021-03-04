#include "args_parser.h"

#include <regex.h>

int parse_options(cmd_args_t *args, int argc, char *argv[], char *envp[]) {
    const char *valid_args = "vcRrwx";
    for (size_t i = 1; i < argc; i++) {
        const char *str = argv[i];
        const size_t str_size = strlen(str);
        char curr[1];
        if (str[0] == '-') {
            for (size_t i = 1; i < str_size; i++) {
                curr[0] = str[i];
                if (strstr(valid_args, curr) == NULL) {
                    return str[i];
                } else {
                    switch (str[i]) {
                        case 'R':
                            args->recursive = true;
                            break;
                        case 'v':
                            args->verbose = true;
                            args->verbose_on_modify = false;
                            break;
                        case 'c':
                            args->verbose_on_modify = !args->verbose;
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
    return 0;
}

int parse_args(cmd_args_t *args, int argc, char *argv[], char *envp[]) {
    memset(args, 0, sizeof(cmd_args_t));
    opterr = 0;

    int opt;
    if ((opt = parse_options(args, argc, argv, envp)) != 0) {
        fprintf(stderr,
                "%c invalid option. Usage: ... inserir usage aqui\n", opt);
        exit(BAD_OPTION);
    }

    return 0;
}
